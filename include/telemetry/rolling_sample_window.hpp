#pragma once
#include <telemetry/telemetry_config.hpp>
#include <telemetry/sensor_data.hpp>
#include <raylib.h>
#include <vector>
#include <chrono>
#include <logging/logger.hpp>
#include <cstdint>

using std::vector;
using std::chrono::microseconds;

using std::uint64_t;
using microsec = uint64_t;

class SampleRingBuffer {
private:

    vector<SensorData> _buffer;
    microsec _bufdur;
    microsec _current_total_dur = 0;

    size_t _next = 0;

    size_t back_one() const {
        return (_next + _buffer.size() - 1) % _buffer.size();
    }

    size_t up_one() const {
        return (_next + _buffer.size() - 1) % _buffer.size();
    }

public:

    SampleRingBuffer(const microsec& buffer_duration_us) : _bufdur(buffer_duration_us) {
        if (buffer_duration_us < 500000) {

            LOG_WARN("Ring buffer created with duration of less than half a second." + 
                    " Creating buffer of one second.");

            _bufdur = ONE_SECOND;
        }

        _buffer.reserve(500);
    }

    bool empty() const {
        return _buffer.empty();
    }

    bool full() const {
        return (_bufdur - _current_total_dur) < 50;
    }

    void add_sample(const SensorData data) {
        if (empty()) {
            _buffer.emplace_back(data);
        }

        auto& prev = _buffer[back_one()];
        microsec time_diff = data.t_us - prev.t_us;

        if (_current_total_dur + time_diff > _bufdur) {

            auto& forward = _buffer[up_one()];
            microsec diff_rm = forward.t_us - _buffer[_next].t_us;
            _current_total_dur -= diff_rm;

            _buffer[_next] = data;

        } else if (_next == 0) {
            _buffer.emplace_back(data);
        } else {
            _buffer.insert(_buffer.begin() + _next, data);
        }

        _current_total_dur += data.t_us;

    }

    Vector3 avg_accel(microsec duration) const {
        if (duration > _bufdur) {
            return avg_accel(_bufdur);
        }

        float sum_x = 0;
        float sum_y = 0;
        float sum_z = 0;
        int samples_read = 0;
        microsec time_count = 0;

        const auto& prev = _buffer[back_one()];
        int start_time = prev.t_us;
        size_t i = back_one();

        while (time_count < duration) {
            auto data = _buffer[i];
            sum_x += data.ax;
            sum_y += data.ay;
            sum_z += data.az;

            samples_read++;
            time_count = data.t_us - start_time;

            i = (_buffer.size() + (i - 1)) % _buffer.size();
        }
        
        return {sum_x / samples_read, sum_y / samples_read, sum_z / samples_read};
    }

    Vector3 avg_accel_all() const {
        return avg_accel(_bufdur);
    }

    Vector3 avg_gyro(microsec duration) const {
        if (duration > _bufdur) {
            return avg_gyro(_bufdur);
        }

        float sum_x = 0;
        float sum_y = 0;
        float sum_z = 0;
        int samples_read = 0;
        microsec time_count = 0;

        const auto& prev = _buffer[back_one()];
        microsec start_time = prev.t_us;
        size_t i = back_one();

        while (time_count < duration) {
            auto data = _buffer[i];
            sum_x += data.gx;
            sum_y += data.gy;
            sum_z += data.gz;

            samples_read++;
            time_count = data.t_us - start_time;

            i = (_buffer.size() + (i - 1)) % _buffer.size();
        }
        
        return {sum_x / samples_read, sum_y / samples_read, sum_z / samples_read};
    }

    Vector3 avg_gyro_all() const {
        return avg_gyro(_bufdur);
    }

    float avg_alt_m(microsec duration) const {
        if (duration > _bufdur) {
            return avg_alt_m(_bufdur);
        }

        float sum = 0;
        int samples_read = 0;
        microsec time_count = 0;

        const auto& prev = _buffer[back_one()];
        microsec start_time = prev.t_us;
        size_t i = back_one();

        while (time_count < duration) {
            auto data = _buffer[i];

            sum += data.altM;

            samples_read++;
            time_count = data.t_us - start_time;

            i = (_buffer.size() + (i - 1)) % _buffer.size();
        }
        
        return sum / samples_read;
    }

    float avg_alt_m_all() const {
        return avg_alt_m(_bufdur);
    }

    int percent_full() {
        float p = (float)_current_total_dur / (float)_bufdur;
        return static_cast<int>(p * 100.0f);
    }
};
