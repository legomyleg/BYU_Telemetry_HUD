#include "logging/data_logger.hpp"
#include "telemetry/mavlink_sensor_parser.hpp"
#include <state/forconverter.hpp>
#include "telemetry/sample_buffer.hpp"
#include "telemetry/sensor_data.hpp"
#include <format>
#include <state/sample_processing.hpp>
#include <cmath>
#include <state/detection/stage_detect.hpp>
#include <raylib.h>
#include "raymath.h"
#include <state/state_update.hpp>
#include <hud/hud_app.hpp>
#include <state/calibration.hpp>
#include <state/rocket_state.hpp>
#include <string>
#include <telemetry/sample_ring_buffer.hpp>
#include <telemetry/telem_source.hpp>
#include <telemetry/telemetry_parse.hpp>
#include <cassert>
#include <vector>
using std::string;

// void ReadSamples(HudApp &app, TelemetrySource &tsrc) {
//     app.data_buffer += tsrc.read_available();
//
//     size_t newline_pos;
//     while((newline_pos = app.data_buffer.find("\n")) != string::npos) {
//         string line = app.data_buffer.substr(0, newline_pos);
//         app.data_buffer.erase(0, newline_pos + 1);
//
//         if (!line.empty()) {
//             try {
//                 SensorData sample = parseLine(line);
//
//                 if (app.state.stage != FlightStage::Calibrating) {
//                     app.sample_queue.push(sample);
//                 }
//                 app.state.sample_buffer.add_sample(sample);
//
//             } catch (...) {
//                 continue;
//             }
//         }
//     }
// }


void handle_sample(HudApp& app, const SensorData& sample) {

    if (app.logger) {
        app.logger->LogSensorData(sample);
    }

    if (app.updateMode == UpdateMode::ReSync) {
        if (sample.t_us < app.resyncArray.back().t_us) {
            app.resyncArray.clear();
            app.resyncArray.push_back(sample);
            return;
        } else if (sample.t_us == app.resyncArray.back().t_us) {
            return;
        }

        app.resyncArray.push_back(sample);

        if (app.resyncArray.size() < 5) {
            return;
        }

        app.sample_queue.clear();
        app.state.sample_buffer.clear();
        app.last_measured_time = 0;

        for (const auto& s : app.resyncArray) {
            app.state.sample_buffer.add_sample(s);
            if (app.state.stage != FlightStage::Calibrating) {
                app.sample_queue.push(s);
            }
        }

        app.resyncArray.clear();
        app.updateMode = UpdateMode::Normal;
        return;
    }

    auto sample_status = app.state.sample_buffer.classify_sample(sample);

    switch (sample_status) {
        case SampleStatus::TimeStall:
            return;

        case SampleStatus::TimeDrop:
            app.updateMode = UpdateMode::ReSync;
            app.resyncArray.clear();
            app.resyncArray.push_back(sample);
            return;

        case SampleStatus::Ok:
            break;
    }


    app.state.sample_buffer.add_sample(sample);
    if (app.state.stage != FlightStage::Calibrating) {
        app.sample_queue.push(sample);
    }
}

void ReadSamples(HudApp& app, TelemetrySource& tsrc) {
    static MavlinkSensorParser parser;
    std::string bytes = tsrc.read_available();
    std::vector<SensorData> samples = parser.push_bytes(bytes);

    for (const SensorData& sample : samples) {
        handle_sample(app, sample);
    }
}

void update_vertical_velocity(float da, float dt_s, float &vert_velocity) {
    vert_velocity = da / dt_s;
}

void update_velocity(SensorData &s, float dt_s, RocketState& state, Biases &biases) {
    Vector3 accel_use;

    ProcessedSample proc_d = process_sample(s);
    Vector3 ab = proc_d.accel_body;
    Vector3 hgb = proc_d.hg_accel_body;

    float total_accel = sqrt(hgb.x*hgb.x + hgb.y*hgb.y + hgb.z*hgb.z);

    if (total_accel < 100) {
        std::cout << std::format("X: {}, Y: {}, Z: {}\n", ab.x, ab.y, ab.z);
        accel_use = Vector3Subtract(ab, biases.accel);
    } else {
        accel_use = Vector3Subtract(hgb, biases.hgaccel);
    }

    Vector3 accel_world = Vector3Transform(accel_use, QuaternionToMatrix(state.orientation));
    Vector3 gravity_world = {0, 0, 9.81};
    Vector3 linear_accel_world = Vector3Subtract(gravity_world, accel_world);

    float dvx = linear_accel_world.x * dt_s;
    float dvy = linear_accel_world.y * dt_s;
    float dvz = linear_accel_world.z * dt_s;

    std::cout << std::format("Corrected: X: {}, Y: {}, Z: {}\n", linear_accel_world.x, linear_accel_world.y, linear_accel_world.z);

    state.velocity.x += dvx;
    state.velocity.y += dvy;
    state.velocity.z += dvz;

    state.total_velocity = sqrtf(
            state.velocity.x * state.velocity.x + 
            state.velocity.y * state.velocity.y +
            state.velocity.z * state.velocity.z
            );
}

void update_orientation(const SensorData& sample, const float dt_s, Quaternion &orientation, Biases &biases) {
    ProcessedSample proc_d = process_sample(sample);
    Vector3 g = proc_d.gyro_body;

    float dx = (g.x - biases.gyro.x) * dt_s;
    float dy = (g.y - biases.gyro.y) * dt_s;
    float dz = (g.z - biases.gyro.z) * dt_s;

    RotationVector rotation = {dx, dy, dz};
    float theta = rotation.length();
    auto norm = rotation.normalized();
    float s = std::sin(theta / 2);

    Quaternion q = {
            norm.x * s,
            norm.y * s,
            norm.z * s,
            cos(theta / 2)
    };

    q = apply_body_transformation_to_render(q);

    orientation = QuaternionMultiply(orientation, q);
    orientation = QuaternionNormalize(orientation);
}

void update_samples_per_sec(float dt_s, float &samples_per_sec) {
    samples_per_sec = 1.0f / dt_s;
}

void UpdateState(HudApp &app, SampleQueue &samples, TelemetrySource &tsrc) {

    static bool runtime_initialized = false;

    ReadSamples(app, tsrc);
    StageDetect::update(app.state);

    if (app.state.stage == FlightStage::Calibrating) return;

    float dt_s;
    float da_m;
    while(!samples.empty()) {
        SensorData data = samples.consume_oldest();

        // Initialize the first sample
        if (app.last_measured_time == 0) {
            app.state.latest_sample = data;
            app.last_measured_time = data.t_us;
            app.state.ASL_altitude = data.altM;
            app.state.AGL_altitude = data.altM - app.state.ground_altitude;
            continue;
        }

        if (data.t_us > app.last_measured_time) {

            dt_s = (data.t_us - app.last_measured_time) / 1000000.0f;
            da_m = data.altM - app.state.ASL_altitude;

            update_orientation(data, dt_s, app.state.orientation, app.state.biases);
            update_velocity(data, dt_s, app.state, app.state.biases);
            update_vertical_velocity(da_m, dt_s, app.state.vertical_velocity_mps);
            update_samples_per_sec(dt_s, app.state.samples_per_sec);

        }


        app.state.ASL_altitude = data.altM;
        app.state.AGL_altitude = data.altM - app.state.ground_altitude;

        app.measuredAlts.push_back({app.state.AGL_altitude, app.last_measured_time/1000000.0f});
        app.last_measured_time = data.t_us;
        app.state.latest_sample = data;
    }

    app.rocket.transform = QuaternionToMatrix(app.state.orientation);
}
