#pragma once
#include <cassert>
#include <cstddef>
#include <telemetry/telemetry_config.hpp>
#include <telemetry/sensor_data.hpp>
#include <raylib.h>
#include <array>
#include <logging/logger.hpp>
#include <format>
#include <cstdint>

using std::array;

using std::uint64_t;
using microsec = uint64_t;

class SampleRingBuffer {
private:
    static constexpr size_t BUFFER_CAPACITY = 200;
    static constexpr microsec SAMPLE_TOLERANCE_US = 40'000;
    array<SensorData, BUFFER_CAPACITY> _buffer;

    size_t head = 0;
    size_t tail = 0;
    size_t sample_count = 0;

    microsec _bufdur;

    const SensorData& front() const {
        assert(!empty());
        return _buffer[(BUFFER_CAPACITY + head - 1) % BUFFER_CAPACITY];
    }

    const SensorData& back() const {
        assert(!empty());
        return _buffer[tail];
    }

public:

    explicit SampleRingBuffer(microsec buffer_duration_us) : _bufdur(buffer_duration_us) {
        LOG_INFO("Buffer created");
        if (buffer_duration_us > 3'000'000) {
            LOG_WARN(std::format("Ring buffer duration too high ({} us). Setting buffer to length 3sec.", buffer_duration_us));
            _bufdur = 3'000'000;
        }
    }

    size_t size() const {
        return sample_count;
    }

    bool empty() const {
        return sample_count == 0;
    }

    void clear() {
        head = 0;
        tail = 0;
        sample_count = 0;
    }

    const SensorData& get_most_recent() {
        return front();
    }

    bool duration_full() const {
        if (sample_count < 2) return false;
        auto current_dur = front().t_us - back().t_us;
        return current_dur + SAMPLE_TOLERANCE_US >= _bufdur;
    }

    void add_sample(const SensorData& data) {

        assert(sample_count == 0 || data.t_us > front().t_us);

        if (sample_count == BUFFER_CAPACITY) {
            // Add error handling here
            
            tail = (tail + 1) % BUFFER_CAPACITY;
            sample_count--;
        }

        _buffer[head] = data;
        head = (head + 1) % BUFFER_CAPACITY;
        sample_count++;

        while (sample_count > 0 && data.t_us - _buffer[tail].t_us > _bufdur) {
            tail = (tail + 1) % BUFFER_CAPACITY;
            sample_count--;
        }
    }

    Vector3 avg_accel(microsec duration) const {

        if (sample_count == 0) return {0,0,0};

        size_t i = (head + BUFFER_CAPACITY - 1) % BUFFER_CAPACITY;
        auto newest_time = _buffer[i].t_us;

        float sum_x = 0;
        float sum_y = 0;
        float sum_z = 0;
        size_t total = 0;

        for (size_t n=0; n < sample_count; n++) {
            const auto &data = _buffer[i];

            if (newest_time - data.t_us > duration) break;

            sum_x += data.ax;
            sum_y += data.ay;
            sum_z += data.az;

            total++;

            i = (BUFFER_CAPACITY + i - 1) % BUFFER_CAPACITY;
        }

        if (total == 0) return {0,0,0};

        return {sum_x / total, sum_y / total, sum_z / total};
    }

    Vector3 avg_accel_all() const {
        return avg_accel(_bufdur);
    }

    Vector3 avg_gyro(microsec duration) const {

        if (sample_count == 0) return {0,0,0};

        size_t i = (head + BUFFER_CAPACITY - 1) % BUFFER_CAPACITY;
        auto newest_time = _buffer[i].t_us;

        float sum_x = 0;
        float sum_y = 0;
        float sum_z = 0;
        size_t total = 0;

        for (size_t n=0; n < sample_count; n++) {
            const auto &data = _buffer[i];

            if (newest_time - data.t_us > duration) break;

            sum_x += data.gx;
            sum_y += data.gy;
            sum_z += data.gz;

            total++;

            i = (BUFFER_CAPACITY + i - 1) % BUFFER_CAPACITY;
        }

        if (total == 0) return {0,0,0};

        return {sum_x / total, sum_y / total, sum_z / total};
    }

    Vector3 avg_gyro_all() const {
        return avg_gyro(_bufdur);
    }

    float avg_alt_m(microsec duration) const {

        if (sample_count == 0) return 0;

        size_t i = (head + BUFFER_CAPACITY - 1) % BUFFER_CAPACITY;
        auto newest_time = _buffer[i].t_us;

        float sum = 0;
        size_t total_samples = 0;

        for (size_t n=0; n < sample_count; n++) {
            const auto &data = _buffer[i];

            if (newest_time - data.t_us > duration) break;

            sum += data.altM;

            total_samples++;

            i = (BUFFER_CAPACITY + i - 1) % BUFFER_CAPACITY;
        }

        if (total_samples == 0) return 0;

        return sum / total_samples;
    }

    float avg_alt_m_all() const {
        return avg_alt_m(_bufdur);
    }

    float avg_vert_vel_mps(microsec duration) const {
        if (sample_count < 2) return 0;

        size_t newest_i = (head + BUFFER_CAPACITY - 1) % BUFFER_CAPACITY;
        const auto newest_t = _buffer[newest_i].t_us;
        const auto newest_alt = _buffer[newest_i].altM;

        auto oldest_i = newest_i;
        for (size_t n=0; n < sample_count; n++) {
            auto candidate = (oldest_i + BUFFER_CAPACITY - 1) % BUFFER_CAPACITY;
            if (newest_t - _buffer[candidate].t_us > duration) break;
            oldest_i = candidate;
        }

        auto dt_s = (newest_t - _buffer[oldest_i].t_us) / 1'000'000.0f;
        auto d_alt = newest_alt - _buffer[oldest_i].altM;
        if (dt_s == 0) return 0;

        return d_alt / dt_s;
    }

    int percent_full() const {
        auto t = front().t_us - back().t_us;
        float p = (float)t / (float)_bufdur;
        return static_cast<int>(p * 100.f);
    }

};
