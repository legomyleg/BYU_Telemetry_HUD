#pragma once
#include "raylib.h"
#include "state/calibration.hpp"
#include "telemetry/sensor_data.hpp"
#include <cstdint>
struct ProcessedSample {
    uint64_t t_us;
    Vector3 accel_body;
    Vector3 gyro_body;
    Vector3 hg_accel_body;
    float altM;
    float pressPa;
};

inline ProcessedSample process_sample(const SensorData& data) {
    ProcessedSample sample;
    sample.t_us = data.t_us;
    sample.altM = data.altM;
    sample.pressPa = data.pressPa;
    Vector3 accel_sensor = {data.ax, data.ay, data.az};
    Vector3 gyro_sensor = {data.gx, data.gy, data.gz};
    Vector3 hg_accel_sensor = {data.hgx, data.hgy, data.hgz};
    sample.accel_body = calibrate_sensor_vector(accel_sensor);
    sample.gyro_body = calibrate_sensor_vector(gyro_sensor);
    sample.hg_accel_body = calibrate_sensor_vector(hg_accel_sensor);

    return sample;
}
