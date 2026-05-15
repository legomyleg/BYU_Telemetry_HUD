#pragma once
#include "telemetry/telem_source.hpp"
#include <string>
using std::string;

class CsvTelemSource : public TelemetrySource {
private:
    int _sampleRate;
    string _filePath;
public:
    CsvTelemSource(int sampleRate, string filePath);
    string read_available() override;
    ~CsvTelemSource();
};
