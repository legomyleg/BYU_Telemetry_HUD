#pragma once
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "telemetry/sensor_data.hpp"
#include "telemetry/rolling_sample_window.hpp"

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

struct Vec3 {
    float x = 0;
    float y = 0;
    float z = 0;
};

struct EulerAngles {
    float roll = 0;
    float pitch = 0;
    float yaw = 0;
};

enum class FlightStage {
    Calibrating,
    Pad,
    Boost,
    Coast,
    Apogee,
    Descent,
    Recovery
};

struct RocketState {
    Quaternion orientation = QuaternionIdentity();
    EulerAngles attitude;
    float altitude = 0;

    Vec3 acceleration;
    float total_acceleration = 0;
    Vec3 velocity;
    float total_velocity = 0;
    float vertical_velocity_mps = 0;
    Vec3 position;
    FlightStage stage = FlightStage::Calibrating;

    SensorData latest_sample{};
    RollingSampleWindow sampleWindow;
};
