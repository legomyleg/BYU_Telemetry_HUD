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
    microseconds _interval;
    microseconds _start_point;
    ifstream _file;

    bool has_read;

    steady_clock::time_point first_read;
    steady_clock::time_point accumulated_time;

    int num_lines();

public:
    CsvTelemSource(uint64_t interval_us, string filePath, uint64_t start_point_us=0);
    string read_available() override;
    ~CsvTelemSource();
};
