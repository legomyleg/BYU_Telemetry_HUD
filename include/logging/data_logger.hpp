#pragma once
#include "telemetry/sensor_data.hpp"
#include <format>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>

using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::time_point_cast;

using std::string;
using std::ofstream;
namespace fs = std::filesystem;

class DataLogger {
private:
    string _file_path;
    ofstream _file;

public:
    DataLogger() {
        fs::path root(ROOT_DIR);
        fs::path log_path = root / "data" / "logged_data";
        
        auto now = time_point_cast<seconds>(system_clock::now());

        string formatted_path = "data_log.csv";

        int append_digit = 0;
        while (fs::exists(log_path / formatted_path)) {
            formatted_path = std::format(
                    "data_log{}.csv", append_digit
            );
            append_digit++;
        }

        auto new_path = log_path / formatted_path;

        _file_path = new_path.string();

        _file = ofstream(_file_path);

        string header = "t_us,ax,ay,az,gx,gy,gz,mx,my,mz,imuTempC,baroTempC,pressPa,altM,hgx,hgy,hgz";

        _file << header << "\n";
    }

    void LogSensorData(const SensorData& data) {
        _file << std::format(
                "{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}\n",
                data.t_us,

                data.ax, data.ay, data.az,
                data.gx, data.gy, data.gz,
                data.mx, data.my, data.mz,
                data.imuTempC,

                data.baroTempC,
                data.pressPa,
                data.altM,

                data.hgx, data.hgy, data.hgz
                );
    }

};
