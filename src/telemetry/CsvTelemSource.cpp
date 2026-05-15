#include "CsvTelemSource.hpp"
#include <iostream>
#include <string>
#include <chrono>
using std::getline, std::string;

CsvTelemSource::CsvTelemSource(int sampleRate, string filePath)
    : _sampleRate(sampleRate), _file(filePath), last_read_time(std::chrono::steady_clock::now()) 
    {
        if (!_file.is_open()) {
            std::cerr << "Error: could not open file at \"" + filePath + "\"" << std::endl;
        }
    }

string CsvTelemSource::read_available() {
    return "";
}
