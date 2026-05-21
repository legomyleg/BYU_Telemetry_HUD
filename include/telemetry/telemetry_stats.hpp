#pragma once

struct TelemetryStats {
    float samples_per_sec = 0;
    int runtime_s = 0;
    int packets_dropped = 0;
};