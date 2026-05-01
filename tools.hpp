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

#include <chrono>
#include <thread>
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

using std::stof, std::stoull;
using std::sqrt;
using std::vector, std::queue;
using std::string, std::stringstream;
using std::ifstream, std::getline;
using std::cerr, std::endl, std::runtime_error;


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
    vector<string> recorded;

    while(getline(ss, dp, ',')) {
        recorded.push_back(dp);
    }
    if (recorded.size() > 17 || recorded.size() < 17) {
        throw std::invalid_argument("Line is not the right size.");
    }

    data.t_us = stoull(recorded[0]);
    for (int i = 1; i < 17; i++) {
        float_points.push(stof(recorded[i]));
    }

    int i = 0;
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

inline void wait(int milli) {
    sleep_for(milliseconds(milli));
}