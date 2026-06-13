#pragma once
#include <cstdint>
#include <telemetry/telem_source.hpp>
#include <chrono>
#include <fstream>
#include <string>
using std::string, std::ifstream;
using std::chrono::microseconds, std::chrono::steady_clock;
using std::chrono::duration_cast;

class CsvTelemSource : public TelemetrySource {
private:
    microseconds _start_point;
    steady_clock::time_point _playback_start;

    string _pending_line;
    uint64_t _pending_t_us = 0;

    ifstream _file;

    bool _has_started;
    bool _has_pending;

    bool load_next_line();

public:
    CsvTelemSource(string filePath, uint64_t start_point_us=0);
    string read_available() override;
    ~CsvTelemSource();
};
