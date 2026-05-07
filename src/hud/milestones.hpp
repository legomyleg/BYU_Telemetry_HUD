#pragma once
#include <string>
#include <vector>
using std::string, std::vector;

// The following are milestones that will be represented by the altitude bar

struct Milestone {
    string name;
    int altitude_m;
};

inline constexpr Milestone UTAH_RECORD = {"UofU Record", 3350};
inline constexpr Milestone BYU_RECORD = {"BYU Rocketry Record", 4000};
inline constexpr Milestone TEN_THOU = {"10,000 feet", 3048};
inline constexpr Milestone TWENTY_THOU = {"20,000 ft", 6096};

inline const vector<Milestone> MILESTONES = {
        UTAH_RECORD,
        BYU_RECORD,
        TEN_THOU,
        TWENTY_THOU
};