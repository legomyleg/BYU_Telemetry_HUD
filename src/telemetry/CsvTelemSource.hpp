#pragma once
#include "../telemetry/telem_source.hpp"
#include <chrono>
#include <fstream>
#include <string>
using std::string, std::ifstream;
using std::chrono::microseconds, std::chrono::steady_clock;
using std::chrono::duration_cast;

class CsvTelemSource : public TelemetrySource {
private:
    microseconds _interval;
    ifstream _file;

    steady_clock::time_point last_read_time;
    steady_clock::time_point accumulated_time;

    int num_lines();

public:
    CsvTelemSource(int interval_us, string filePath);
    string read_available() override;
    ~CsvTelemSource();
};
