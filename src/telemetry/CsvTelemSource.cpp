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

CsvTelemSource::CsvTelemSource(string filePath, uint64_t start_point_us)
    : _file(filePath), _start_point(start_point_us)
{
    if (!_file.is_open()) {
        std::cerr << "Error: could not open file at \"" + filePath + "\"" << std::endl;
    }

    _has_started = false;

    string throw_away_header;
    getline(_file, throw_away_header);
        
    load_next_line();
    if (start_point_us == 0 && _has_pending) {
        _start_point = microseconds(_pending_t_us);
    } else {
        while (_has_pending && _pending_t_us < _start_point.count()) {
            load_next_line();
        }
    }
}

bool CsvTelemSource::load_next_line() {
    if (!getline(_file, _pending_line)) {
        _has_pending = false;
        return false;
    }

    _pending_t_us = get_t_us(_pending_line);
    _has_pending = true;
    return true;
}

string CsvTelemSource::read_available() {

    if (!_has_started) {
        _playback_start = steady_clock::now();
        _has_started = true;
    }

    auto elapsed = duration_cast<microseconds>(steady_clock::now() - _playback_start);
    uint64_t playback_t_us = _start_point.count() + elapsed.count();

    string lines;

    while (_has_pending && _pending_t_us <= playback_t_us) {
        lines.append(_pending_line).append("\n");
        load_next_line();
    }

    return lines;
}

CsvTelemSource::~CsvTelemSource() = default;
