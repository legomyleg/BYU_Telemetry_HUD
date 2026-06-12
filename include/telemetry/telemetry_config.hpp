#pragma once

#include <cstdint>
#include <string_view>
#include <termios.h>
using std::uint64_t;

inline constexpr std::string_view PORT_NAME = "/dev/cu.usbmodemN32G45x1";
inline constexpr speed_t BAUD_RATE = 115200;

inline constexpr std::string_view RTSP_URL = "rtsp://192.168.144.25:8554/main.264";
// inline constexpr std::string_view RTSP_URL = "rtsp://localhost:8554/mystream";

inline constexpr int SAMPLE_RATE = 60;
inline constexpr int EXPECTED_FLIGHT_TIME_S = 300;
inline constexpr int EXPECTED_MAX_ALT_M = 10000;

inline constexpr uint64_t HALF_SECOND = 500000;
inline constexpr uint64_t ONE_SECOND = 1000000;
inline constexpr uint64_t ONE_ONE_HALF_SECOND = 1500000;
inline constexpr uint64_t TWO_SECOND = 2000000;
