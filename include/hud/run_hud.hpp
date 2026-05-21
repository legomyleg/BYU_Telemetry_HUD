#pragma once
#include "../telemetry/telem_source.hpp"
#include <chrono>

void RunHud(TelemetrySource *data_src, std::chrono::microseconds buffer_size, bool no_calibrate=false);
