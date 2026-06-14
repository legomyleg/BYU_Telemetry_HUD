#pragma once
#include <string>
using std::string;

class TelemetrySource {
public:
    virtual string read_available() = 0;
    virtual void discard_available() {
        read_available();
    }
    virtual ~TelemetrySource() = default;
};
