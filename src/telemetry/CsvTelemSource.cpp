#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <telemetry/CsvTelemSource.hpp>
#include <iostream>
#include <string>
#include <chrono>
using std::getline, std::string;
using std::chrono::microseconds, std::chrono::steady_clock;
using std::chrono::duration_cast;

uint64_t get_t_us(string line) {
    std::stringstream ss(line);
    string t_str;
    getline(ss, t_str, ',');
    uint64_t t;
    try {
        t = std::stoull(t_str);
    } catch (...) {
        throw std::runtime_error("Could not parse string " + t_str);
    }
    return t;
}

CsvTelemSource::CsvTelemSource(uint64_t interval_us, string filePath, uint64_t start_point_us)
    : _interval(interval_us), _file(filePath), accumulated_time(steady_clock::now()), _start_point(start_point_us)
{
    if (!_file.is_open()) {
        std::cerr << "Error: could not open file at \"" + filePath + "\"" << std::endl;
    }
    has_read = false;
    
    bool start_reached = false;
    string line;
    while (!start_reached) {
        getline(_file, line);
        auto t = get_t_us(line);
        start_reached = t < _start_point.count() ? false : true;
    }
}

int CsvTelemSource::num_lines() {
    auto elapsed = steady_clock::now() - accumulated_time;
    microseconds delta = duration_cast<microseconds>(elapsed);
    int num_lines = static_cast<int>(delta / _interval);
    accumulated_time += (_interval * num_lines);
    return num_lines;
}

string CsvTelemSource::read_available() {

    if (!has_read) {
        accumulated_time = steady_clock::now();
        has_read = true;
    }

    string lines;
    int nlines = num_lines();

    int lines_read = 0;
    string temp_str;

    while (lines_read++ < nlines && getline(_file, temp_str)) {
        lines.append(temp_str).append("\n");
    }

    return lines;
}

CsvTelemSource::~CsvTelemSource() = default;
