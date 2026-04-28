#pragma once
#include <cmath>
#include <vector>
#include <queue>

using std::sqrt;
using std::vector, std::queue;


struct GyroSample {
    long long time_elapsed_us;
    float gx;
    float gy;
    float gz;
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

struct IMU_Data {
    long long t_us;
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float mx;
    float my;
    float mz;
    float imuTempC;
    float baroTempC;
    float pressPa;
    float altM;
    float hgx;
    float hgy;
    float hgz;


};

class DataRows {
private:
    queue<IMU_Data> rows;

public:

    IMU_Data consume_row() {
        if (rows.empty()) return {};

        auto row = rows.front();
        rows.pop();
        return row;
    }

    void push(IMU_Data data) {
        rows.push(data);
    }

    bool empty() {
        return rows.empty();
    }
};

struct FrameData {
    IMU_Data imu_data;
};