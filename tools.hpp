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
using std::stof, std::stoll;
using std::vector, std::queue;
using std::string, std::stringstream;
using std::ifstream, std::getline;
using std::cerr, std::endl, std::runtime_error;

inline void update_orientation(const GyroSample sample, const long long dt_us, Quaternion& orientation) {
    long long dt = dt_us / 1000000.0f;
    float dx = sample.gx * dt;
    float dy = sample.gy * dt;
    float dz = sample.gz * dt;

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

inline vector<float IMU_Data::*> float_fields = {
    &IMU_Data::ax,
    &IMU_Data::ay,
    &IMU_Data::az,
    &IMU_Data::gx,
    &IMU_Data::gy,
    &IMU_Data::gz,
    &IMU_Data::mx,
    &IMU_Data::my,
    &IMU_Data::mz,
    &IMU_Data::imuTempC,
    &IMU_Data::baroTempC,
    &IMU_Data::pressPa,
    &IMU_Data::altM,
    &IMU_Data::hgx,
    &IMU_Data::hgy,
    &IMU_Data::hgz
};

inline IMU_Data parseLine(const string& line) {
    IMU_Data data;

    stringstream ss(line);
    string dp;
    queue<float> float_points;
    long long time;

    int i = 0;
    while(getline(ss, dp, ',')) {
        if (i++ == 0) {
            time = stoll(dp);
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

inline DataRows get_data(const string& file_path) {
    DataRows data;
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