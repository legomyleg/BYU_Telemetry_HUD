#include <telemetry/sensor_data.hpp>
#include <format>
#include <string>
#include <telemetry/telem_source.hpp>
using std::string;

class TestingTelemSrc : public TelemetrySource {
private:
    string internal_buffer;
public: 
    void insert_data_str(string data) {
        internal_buffer = internal_buffer.append(data);
    }
    void insert_data(SensorData data) {
        string data_str = std::format(
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
        internal_buffer = internal_buffer.append(data_str);
    }
    string read_available() override {
        string read_out = internal_buffer;
        internal_buffer = "";
        return read_out;
    }

};
