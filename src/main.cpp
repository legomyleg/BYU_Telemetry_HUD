#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <state/rocket_state.hpp>
#include <hud/draw/home_window.hpp>
#include <telemetry/telemetry_config.hpp>
#include <chrono>

int main() {
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);

    auto buf_size = std::chrono::microseconds(TWO_SECOND);
    RunHud(&serial, buf_size);
}
