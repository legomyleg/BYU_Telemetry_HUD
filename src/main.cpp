#include "hud/run_hud.hpp"
#include "hud/hud_app.hpp"
#include "state/rocket_state.hpp"
#include "hud/draw/home_window.hpp"

int main() {
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);
    RunHud(&serial);
}
