#include "state/calibration.hpp"
#include "telemetry/mavlink_sensor_parser.hpp"
#include "telemetry/sensor_data.hpp"
#include "telemetry/serial.hpp"
#include "telemetry/telemetry_config.hpp"
#include <format>
#include <iostream>
#include <raylib.h>
#include <vector>
int main() {
    SerialPort serial(PORT_NAME, BAUD_RATE);

    static MavlinkSensorParser parser;
    while (true) {
        auto bytes = serial.read_available();
        std::vector<SensorData> samples = parser.push_bytes(bytes);

        for (const SensorData& sample : samples) {
            Vector3 accel = {sample.ax, sample.ay, sample.az};
            Vector3 gyro = {sample.gz, sample.gy, sample.gz};
            auto t = calibrate_sensor_vector(accel);
            
            // std::cout << std::format(
            //         "T_us: {}, Accel: [{:.2f}, {:.2f}, {:.2f}] -- Gyro: [{:.2f}, {:.2f}, {:.2f}]\n",
            //         sample.t_us,
            //         sample.ax, sample.ay, sample.az,
            //         sample.gx, sample.gy, sample.gz
            //         );

            std::cout << std::format(
                    "{}\n", sample.t_us
                    );

            // std::cout << std::format(
            //         "Frames: {}, Frames Ignored: {}\n",
            //         parser.stats().frames_crc, parser.stats().frames_ignored
            //         );


            // std::cout << std::format(
            //         "Transformed: Accel: [{:.2f}, {:.2f}, {:.2f}] -- Gyro: [{:.2f}, {:.2f}, {:.2f}]\n",
            //         t.x, t.y, t.z,
            //         sample.gx, sample.gy, sample.gz
            //         );
        }
    }
}
