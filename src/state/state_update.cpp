#include <cmath>
#include <state/detection/stage_detect.hpp>
#include <raylib.h>
#include <state/state_update.hpp>
#include <hud/hud_app.hpp>
#include <state/calibration.hpp>
#include <state/rocket_state.hpp>
#include <telemetry/sample_ring_buffer.hpp>
#include <telemetry/telem_source.hpp>
#include <telemetry/telemetry_parse.hpp>
#include <cassert>

void ReadSamples(HudApp &app, TelemetrySource &tsrc) {
    app.data_buffer += tsrc.read_available();

    size_t newline_pos;
    while((newline_pos = app.data_buffer.find("\n")) != string::npos) {
        string line = app.data_buffer.substr(0, newline_pos);
        app.data_buffer.erase(0, newline_pos + 1);

        if (!line.empty()) {
            try {
                SensorData sample = parseLine(line);
                if (app.state.stage != FlightStage::Calibrating) {
                    app.sample_queue.push(sample);
                }
                app.state.sample_buffer.add_sample(sample);

            } catch (...) {
                continue;
            }
        }
    }
}

void update_vertical_velocity(float da, float dt_s, float &vert_velocity) {
    vert_velocity = da / dt_s;
}

void update_velocity(SensorData &s, float dt_s, RocketState& state, Biases &biases) {
    float ax_use, ay_use, az_use;

    float total_accel = sqrt(s.hgx*s.hgx + s.hgy*s.hgy + s.hgz*s.hgz);

    if (total_accel > 100) {
        ax_use = s.hgx;
        ay_use = s.hgy;
        az_use = s.hgz - 9.81;
    } else {
        ax_use = s.ax - biases.accel.x;
        ay_use = s.ay - biases.accel.y;
        az_use = s.az - biases.accel.z;
    }

    float dvx = ax_use * dt_s;
    float dvy = ay_use * dt_s;
    float dvz = az_use * dt_s;

    state.velocity.x += dvx;
    state.velocity.y += dvy;
    state.velocity.z += dvz;

    state.total_velocity = sqrtf(
            state.velocity.x * state.velocity.x + 
            state.velocity.y * state.velocity.y +
            state.velocity.z * state.velocity.z
            );
}

void update_orientation(const SensorData sample, const float dt_s, Quaternion &orientation, Biases &biases) {
    float dx = (sample.gx - biases.gyro.x) * dt_s;
    float dy = (sample.gy - biases.gyro.y) * dt_s;
    float dz = (sample.gz - biases.gyro.z) * dt_s;

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
            update_samples_per_sec(dt_s, app.telemetry.samples_per_sec);

        }


        app.state.ASL_altitude = data.altM;
        app.state.AGL_altitude = data.altM - app.state.ground_altitude;

        app.measuredAlts.push_back({app.state.AGL_altitude, app.last_measured_time/1000000.0f});
        app.last_measured_time = data.t_us;
        app.state.latest_sample = data;
    }

    app.rocket.transform = QuaternionToMatrix(app.state.orientation);
}
