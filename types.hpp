#pragma once
#include <cmath>
#include <vector>
#include <queue>

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

struct Velocity {
    float vx, vy, vz;
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

struct FrameData {

    SensorData sensor_data;
};

struct TestFrameData : FrameData {

};

struct RocketState {
    Quaternion orientation;
    Velocity velocity;
    
};
