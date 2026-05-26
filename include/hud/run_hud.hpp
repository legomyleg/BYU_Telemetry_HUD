#pragma once
#include <telemetry/telem_source.hpp>

void RunHud(TelemetrySource *data_src, uint64_t buffer_size, bool no_calibrate=false);
