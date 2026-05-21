#pragma once

#include <telemetry/telem_source.hpp>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <thread>
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

#include <stdexcept>
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::runtime_error;

class SerialPort : public TelemetrySource {
private:
    int fd = -1;

public:
    SerialPort(const string& port_name, speed_t baud_rate = B9600);

    ~SerialPort();

    string read_available() override;
};
