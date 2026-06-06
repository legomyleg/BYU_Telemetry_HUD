#pragma once
#include "hud/config.hpp"
#include <string>
#include <vector>
#include <chrono>
using std::string, std::vector;

// DEBUG
using std::chrono::system_clock;
using std::chrono::time_point_cast;
using std::chrono::seconds;

#include <iostream>
using std::cout;

#include <format>
using std::format;
// DEBUG

// The following are milestones that will be represented by the altitude bar

struct Milestone {
    const string name;
    const int altitude_m;
    bool reached = false;
};

// 23276 ft is the BYU Record
inline Milestone BYU_RECORD = {"BYU Rocketry Record", 7095};
inline Milestone TEN_THOU = {"10,000 feet", 3048};
inline Milestone TWENTY_THOU = {"20,000 ft", 6096};
inline Milestone BAR_TOP = {"", 0};

inline vector<Milestone> MILESTONES = {
        BYU_RECORD,
        TEN_THOU,
        TWENTY_THOU
};
