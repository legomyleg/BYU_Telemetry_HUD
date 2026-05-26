#include <cstdint>
#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <state/rocket_state.hpp>
#include <hud/draw/home_window.hpp>
#include <telemetry/telemetry_config.hpp>

int main() {
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);

    uint64_t buf_size = 2'000'000;
    RunHud(&serial, buf_size);
}
