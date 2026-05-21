#pragma once
#include <telemetry/sensor_data.hpp>
#include <raylib.h>
#include <vector>
#include <chrono>
using std::vector;
using std::chrono::microseconds;

class RollingSampleWindow {
private:
    vector<SensorData> _window;
    microseconds _buffer_duration;
    int _incurred_time;
    int _ilatest = 0;
    int _next = 0;

    void window_insert(int i, const SensorData& data) {
        if (i == 0) {
            _window.emplace_back(data);
        } else {
            _window.insert(_window.begin() + i, data);
        }
    }

public:

    RollingSampleWindow(microseconds buf_duration) : _buffer_duration(buf_duration) {}

    bool full() const {
        return (_buffer_duration.count() - _incurred_time) < 50;
    }

    bool empty() const {
        return _window.empty();
    }

    SensorData latest() const {
        return _window[_ilatest];
    }

    void add_sample(SensorData data) {
        int time_increase = data.t_us - latest().t_us;
        _incurred_time += time_increase;

        if ((_incurred_time + time_increase) > _buffer_duration.count()) {

            int back_i = _next;
            int one_up = back_i + 1 % _window.size();
            _incurred_time -= _window[one_up].t_us - _window[back_i].t_us;

            
            _window[_next] = data;

            _ilatest = _next;
            _next = _next + 1 % _window.size();

        } else {

            window_insert(_next, data);
            _ilatest = _next;
            _next = _next + 1 % _window.size();

        }
    }

    Vector3 avg_accel(const microseconds& duration) const {
        if (duration > _buffer_duration) {
            return avg_accel(_buffer_duration);
        }

        float sum_x, sum_y, sum_z;
        int samples_read;
        int time_count;
        int start_time = latest().t_us;
        int i = _ilatest;

        while (time_count < _buffer_duration.count()) {
            auto data = _window[i];
            sum_x += data.ax;
            sum_y += data.ay;
            sum_z += data.az;

            samples_read++;
            time_count = data.t_us - start_time;

            i = _window.size() + (i - 1) % _window.size();
        }
        
        return {sum_x / samples_read, sum_y / samples_read, sum_z / samples_read};
    }

    Vector3 avg_accel_all() const {
        return avg_accel(_buffer_duration);
    }

    Vector3 avg_gyro(const microseconds& duration) const {
        if (duration > _buffer_duration) {
            return avg_gyro(_buffer_duration);
        }

        float sum_x, sum_y, sum_z;
        int samples_read;
        int time_count;
        int start_time = latest().t_us;
        int i = _ilatest;

        while (time_count < _buffer_duration.count()) {
            auto data = _window[i];
            sum_x += data.gx;
            sum_y += data.gy;
            sum_z += data.gz;

            samples_read++;
            time_count = data.t_us - start_time;

            i = _window.size() + (i - 1) % _window.size();
        }
        
        return {sum_x / samples_read, sum_y / samples_read, sum_z / samples_read};
    }

    Vector3 avg_gyro_all() const {
        return avg_gyro(_buffer_duration);
    }

    float avg_alt_m(const microseconds& duration) const {
        if (duration > _buffer_duration) {
            return avg_alt_m(_buffer_duration);
        }

        float sum;
        int samples_read;
        int time_count;
        int start_time = latest().t_us;
        int i = _ilatest;

        while (time_count < _buffer_duration.count()) {
            auto data = _window[i];

            sum += data.altM;

            samples_read++;
            time_count = data.t_us - start_time;

            i = _window.size() + (i - 1) % _window.size();
        }
        
        return sum / samples_read;
    }

    float avg_alt_m_all() const {
        return avg_alt_m(_buffer_duration);
    }

    int percent_full() const {
        float p = (float)_buffer_duration.count() / (float)_incurred_time;
        return static_cast<int>(p * 100.0f);
    }

};
