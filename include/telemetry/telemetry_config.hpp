#pragma once

#include <cstdint>
using std::uint64_t;

inline constexpr int SAMPLE_RATE = 60;
inline constexpr int EXPECTED_FLIGHT_TIME_S = 300;
inline constexpr int EXPECTED_MAX_ALT_M = 10000;

inline constexpr uint64_t HALF_SECOND = 500000;
inline constexpr uint64_t ONE_SECOND = 1000000;
inline constexpr uint64_t ONE_ONE_HALF_SECOND = 1500000;
inline constexpr uint64_t TWO_SECOND = 2000000;
