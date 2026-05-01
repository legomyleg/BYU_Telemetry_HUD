#pragma once
#include <cmath>
#include <vector>
#include <queue>
#include "raylib.h"
#include "raymath.h"

using std::sqrt;
using std::vector, std::queue;

struct MagnetometerSample {
    float mx;
    float my;
    float mz;
};

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

struct SensorData {
  uint64_t t_us;

  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float imuTempC;

  float baroTempC;
  float pressPa;
  float altM;

  float hgx, hgy, hgz;
};

class SampleBuffer {
private:
    queue<SensorData> rows;

public:

    SensorData consume_latest() {
        if (rows.empty()) return {};

        SensorData latest;
        while(!rows.empty()) {
            latest = rows.front();
            rows.pop();
        }

        return latest;
    }

    SensorData consume_oldest() {
        auto data = rows.front();
        rows.pop();
        return data;
    }

    SensorData latest() const {
        if (rows.empty()) return {};
        return rows.back();
    }

    void reduce_to_latest() {
        if (rows.empty() || rows.size() == 1) return;

        while (rows.size() > 1) {
            rows.pop();
        }
    }

    void push(SensorData data) {
        rows.push(data);
    }

    bool empty() const {
        return rows.empty();
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

    SensorData latest_sample{};
};
