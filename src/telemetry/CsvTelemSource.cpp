#include <telemetry/CsvTelemSource.hpp>
#include <iostream>
#include <string>
#include <chrono>
using std::getline, std::string;
using std::chrono::microseconds, std::chrono::steady_clock;
using std::chrono::duration_cast;

CsvTelemSource::CsvTelemSource(int interval_us, string filePath)
    : _interval(interval_us), _file(filePath), accumulated_time(steady_clock::now())
{
    if (!_file.is_open()) {
        std::cerr << "Error: could not open file at \"" + filePath + "\"" << std::endl;
    }
    has_read = false;
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

    int nlines = num_lines();
    string lines;

    int lines_read = 0;
    string temp_str;

    while (lines_read++ < nlines && getline(_file, temp_str)) {
        lines.append(temp_str).append("\n");
    }

    return lines;
}

CsvTelemSource::~CsvTelemSource() = default;
