#pragma once
#include <telemetry/telem_source.hpp>
#include <logging/data_logger.hpp>

void RunHud(TelemetrySource *data_src, uint64_t buffer_size, bool no_calibrate=false, bool get_feed=true);
