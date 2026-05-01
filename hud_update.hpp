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
            SensorData sample = parseLine(line);
            current_data.push(sample);
        }
    }
}

inline void UpdateHUD(HudApp& app, SampleBuffer& samples) {
    uint64_t time_prev = app.last_measured_time;
    float dt_s;
    while(!samples.empty()) {
        SensorData data = samples.consume_oldest();

        if (time_prev != 0 && data.t_us > time_prev) {
            dt_s = (data.t_us - time_prev) / 1000000.0f;
            update_orientation(data, dt_s, app.orientation);
        }

        // Do velocity calculations here

        time_prev = data.t_us;
    }

    app.last_measured_time = time_prev;
    app.rocket.transform = QuaternionToMatrix(app.orientation);
}