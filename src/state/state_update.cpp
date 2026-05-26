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
                app.sample_buffer.push(sample);
                app.state.sampleWindow.add_sample(sample);
            } catch (...) {
                continue;
            }
        }
    }
}

void update_vertical_velocity(float da, float dt_s, float &vert_velocity) {
    vert_velocity = da / dt_s;
}

void update_velocity(SensorData &s, float dt_s, Vec3 &velocity, Biases &biases) {
    float ax_use, ay_use, az_use;

    float total_accel = sqrt(s.hgx*s.hgx + s.hgy*s.hgy + s.hgz*s.hgz);

    if (total_accel > 100) {
        ax_use = s.hgx;
        ay_use = s.hgy;
        az_use = s.hgz - 9.81;
    } else {
        ax_use = s.ax - biases.accel.x;
        ay_use = s.ay - biases.accel.y;
        az_use = s.az - biases.accel.z - 9.81;
    }

    velocity.x = ax_use * dt_s;
    velocity.y = ay_use * dt_s;
    velocity.z = az_use * dt_s;
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

void UpdateState(HudApp &app, SampleBuffer &samples, TelemetrySource &tsrc) {

    ReadSamples(app, tsrc);

    if (!app.state.sampleWindow.duration_full()) {
        return;
    } else if (app.state.stage == FlightStage::Calibrating) {

        // Calibrating the biases
        app.biases.accel = app.state.sampleWindow.avg_accel_all();
        app.biases.gyro = app.state.sampleWindow.avg_gyro_all();
        app.state.ground_altitude = app.state.sampleWindow.avg_alt_m_all();

        app.state.stage = FlightStage::Pad;
    }

    float dt_s;
    float da_m;
    while(!samples.empty()) {
        SensorData data = samples.consume_oldest();

        // Initialize the first sample
        if (app.last_measured_time == 0) {
            app.state.latest_sample = data;
            app.last_measured_time = data.t_us;
            app.state.altitude = data.altM;
            continue;
        }

        if (data.t_us > app.last_measured_time) {
            dt_s = (data.t_us - app.last_measured_time) / 1000000.0f;
            da_m = data.altM - app.state.altitude;

            update_orientation(data, dt_s, app.state.orientation, app.biases);
            update_velocity(data, dt_s, app.state.velocity, app.biases);
            update_vertical_velocity(da_m, dt_s, app.state.vertical_velocity_mps);
            update_samples_per_sec(dt_s, app.telemetry.samples_per_sec);

        }
        app.state.latest_sample = data;
        app.state.altitude = data.altM;
        app.last_measured_time = data.t_us;
        app.measuredAlts.push_back({app.state.altAGL(), app.last_measured_time/1000000.0f});
    }

    app.rocket.transform = QuaternionToMatrix(app.state.orientation);
}
