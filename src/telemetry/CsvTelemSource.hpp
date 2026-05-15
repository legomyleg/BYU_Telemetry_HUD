#pragma once
#include "telemetry/telem_source.hpp"
#include <chrono>
#include <fstream>
#include <string>
using std::string, std::ifstream;

class CsvTelemSource : public TelemetrySource {
private:
    int _sampleRate;
    ifstream _file;

    std::chrono::steady_clock::time_point last_read_time;
    std::chrono::microseconds accumulated_time;

public:
    CsvTelemSource(int sampleRate, string filePath);
    string read_available() override;
    ~CsvTelemSource();
};
