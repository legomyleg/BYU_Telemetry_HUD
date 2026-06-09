#include <cstdint>
#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <state/rocket_state.hpp>
#include <hud/draw/home_window.hpp>
#include <telemetry/telemetry_config.hpp>

int main() {
    SerialPort serial = SerialPort(PORT_NAME, BAUD_RATE);

    uint64_t buf_size = 2'000'000;
    RunHud(&serial, buf_size);
}
