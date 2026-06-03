#pragma once

#include <vector>
#include <queue>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include "sensor_data.hpp"
#include "sample_buffer.hpp"
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

extern vector<float SensorData::*> float_fields;

SensorData parseLine(const string& line);

SampleQueue get_data(const string& file_path);

void wait(int milli);
