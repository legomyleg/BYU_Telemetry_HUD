#pragma once
#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <raymath.h>
#include <telemetry/sensor_data.hpp>
#include <telemetry/sample_ring_buffer.hpp>
#include <state/calibration.hpp>
#include <string>
#include <type_traits>
using std::string;

using std::sqrt;

struct RotationVector {
    float x;
    float y;
    float z;

    float length() const {
        return sqrt(x*x + y*y + z*z);
    }

    RotationVector normalized() const {
        float len = length();

        if (len < 1e-8f) {
            return {0.0f, 0.0f, 0.0f};
        }

        return {
            x / len,
            y / len,
            z / len
        };
    }
};

struct EulerAngles {
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
};

enum class FlightStage {
    Calibrating = 10,
    Pad = 20,
    Boost = 30,
    Coast = 40,
    Apogee = 50,
    Descent = 60,
    Recovery = 70
};

struct StageInfo {
    FlightStage stage;
    const char* label;
};

constexpr StageInfo STAGES[] = {
        {FlightStage::Pad, "Pad"},
        {FlightStage::Boost, "Boost"},
        {FlightStage::Coast, "Coast"},
        {FlightStage::Apogee, "Apogee"},
        {FlightStage::Descent, "Descent"},
        {FlightStage::Recovery, "Recovery"}
};

struct RocketState {
    Quaternion orientation = QuaternionIdentity();
    EulerAngles attitude;
    float ground_altitude = 0;
    float ASL_altitude = 0;
    float AGL_altitude = 0;

    Biases biases;

    Vector3 acceleration;
    float total_acceleration = 0;
    Vector3 velocity;
    float total_velocity = 0;
    float vertical_velocity_mps = 0;
    Vector3 position;
    FlightStage stage = FlightStage::Calibrating;

    SensorData latest_sample{};
    SampleRingBuffer sample_buffer;

    uint64_t stage_start_us = 0;

    void transition_to(FlightStage new_stage) {
        using StageValue = std::underlying_type_t<FlightStage>;

        if (new_stage < stage) {
            return;
        } else if (StageValue(new_stage) - StageValue(stage) > 10) {
            return;
        }

        stage = new_stage;
        stage_start_us = sample_buffer.get_most_recent().t_us;
    }

    uint64_t time_in_stage() {
        return sample_buffer.get_most_recent().t_us - stage_start_us;
    }

    RocketState(uint64_t buffer_size) : sample_buffer(buffer_size) {}
};
