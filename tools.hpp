#pragma once

#include <vector>
#include <queue>
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "types.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using std::stof, std::stoull;
using std::sqrt;
using std::vector, std::queue;
using std::string, std::stringstream;
using std::ifstream, std::getline;
using std::cerr, std::endl, std::runtime_error;

inline void update_orientation(const SensorData sample, const float dt_s, Quaternion& orientation) {
    float dx = sample.gx * dt_s;
    float dy = sample.gy * dt_s;
    float dz = sample.gz * dt_s;

    RotationVector rotation = {dx, dy, dz};
    float theta = rotation.length();
    auto norm = rotation.normalized();
    float s = std::sin(theta / 2);

    Quaternion q = {
        norm.x * s,
        norm.y * s,
        norm.z * s,
        cos(theta / 2)
    };

    orientation = QuaternionMultiply(orientation, q);
    orientation = QuaternionNormalize(orientation);
}

inline Velocity calculate_velocity(SensorData &s, float dt_s) {
    float ax_use, ay_use, az_use;

    float total_accel = sqrt(s.hgx*s.hgx + s.hgy*s.hgy + s.hgz*s.hgz);

    if (total_accel > 100) {
        ax_use = s.hgx;
        ay_use = s.hgy;
        az_use = s.hgz - 9.81;
    } else {
        ax_use = s.ax;
        ay_use = s.ay;
        az_use = s.az - 9.81;
    }

    return {ax_use * dt_s, ay_use * dt_s, az_use * dt_s};
}

inline vector<float SensorData::*> float_fields = {
    &SensorData::ax,
    &SensorData::ay,
    &SensorData::az,
    &SensorData::gx,
    &SensorData::gy,
    &SensorData::gz,
    &SensorData::mx,
    &SensorData::my,
    &SensorData::mz,
    &SensorData::imuTempC,
    &SensorData::baroTempC,
    &SensorData::pressPa,
    &SensorData::altM,
    &SensorData::hgx,
    &SensorData::hgy,
    &SensorData::hgz
};

inline SensorData parseLine(const string& line) {
    std::cout << line << std::endl;
    SensorData data;

    stringstream ss(line);
    string dp;
    queue<float> float_points;
    uint64_t time;

    int i = 0;
    while(getline(ss, dp, ',')) {
        if (i++ == 0) {
            time = stoull(dp);
        } else {
            float_points.push(stof(dp));
        }
    }

    data.t_us = time;

    i = 0;
    while (!float_points.empty()) {
        data.*float_fields[i++] = float_points.front();
        float_points.pop();
    }

    return data;
}

inline SampleBuffer get_data(const string& file_path) {
    SampleBuffer data;
    ifstream inFile(file_path);
    string line;
    if (!inFile.is_open()) {
        throw runtime_error("Could not open file at " + file_path);
    }

    getline(inFile, line);
    while (getline(inFile, line)) {
        data.push(parseLine(line));
    }

    return data;
}