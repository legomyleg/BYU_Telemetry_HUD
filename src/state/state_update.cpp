#include "state_update.hpp"
#include "telemetry/telemetry_parse.hpp"

void ReadSerialSamples(HudApp& app, SampleBuffer& current_data, SerialPort &serial) {
    app.serial_buffer += serial.read_available();

    size_t newline_pos;
    while((newline_pos = app.serial_buffer.find("\n")) != string::npos) {
        string line = app.serial_buffer.substr(0, newline_pos);
        app.serial_buffer.erase(0, newline_pos + 1);

        if (!line.empty()) {
            try {
                SensorData sample = parseLine(line);
                current_data.push(sample);
            } catch (...) {
                continue;
            }
        }
    }
}

void update_vertical_velocity(float da, float dt_s, float &vert_velocity) {
    vert_velocity = da / dt_s;
}

void update_velocity(SensorData &s, float dt_s, Vec3 &velocity) {
    float ax_use, ay_use, az_use;

    float total_accel = sqrt(s.hgx*s.hgx + s.hgy*s.hgy + s.hgz*s.hgz);

    if (total_accel > 100) {
        ax_use = s.hgx;
        ay_use = s.hgy;
        az_use = s.hgz - 9.81;
    } else {
        ax_use = s.ax;
        ay_use = s.ay;
        az_use = s.az - 9.81;
    }

    velocity.x = ax_use * dt_s;
    velocity.y = ay_use * dt_s;
    velocity.z = az_use * dt_s;
}

void update_orientation(const SensorData sample, const float dt_s, Quaternion& orientation) {
    float dx = sample.gx * dt_s;
    float dy = sample.gy * dt_s;
    float dz = sample.gz * dt_s;

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

void UpdateState(HudApp& app, SampleBuffer& samples, SerialPort& serial) {
    ReadSerialSamples(app, app.running_data, serial);

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

            update_orientation(data, dt_s, app.state.orientation);
            update_velocity(data, dt_s, app.state.velocity);
            update_vertical_velocity(da_m, dt_s, app.state.vertical_velocity_mps);
        }
        app.state.latest_sample = data;
        app.state.altitude = data.altM;
        app.last_measured_time = data.t_us;
    }

    app.rocket.transform = QuaternionToMatrix(app.state.orientation);
}
