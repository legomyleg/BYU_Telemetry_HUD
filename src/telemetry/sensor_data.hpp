#pragma once
#include <cstdint>

struct MagnetometerSample {
    float mx;
    float my;
    float mz;
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
