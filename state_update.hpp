#pragma once
#include "hud_setup.hpp"
#include "types.hpp"
#include "tools.hpp"

inline void ReadSerialSamples(HudApp& app, SampleBuffer& current_data, SerialPort &serial) {
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

inline void update_vertical_velocity() {

}

inline void UpdateState(HudApp& app, SampleBuffer& samples) {
    uint64_t time_prev = app.last_measured_time;
    float dt_s;
    while(!samples.empty()) {
        SensorData data = samples.consume_oldest();

        if (time_prev != 0 && data.t_us > time_prev) {
            dt_s = (data.t_us - time_prev) / 1000000.0f;
            update_orientation(data, dt_s, app.state.orientation);
        }

        // Do velocity calculations here

        time_prev = data.t_us;
    }

    app.last_measured_time = time_prev;
    app.rocket.transform = QuaternionToMatrix(app.state.orientation);
}