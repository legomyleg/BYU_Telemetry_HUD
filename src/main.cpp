#include "raylib.h"
#include "hud/hud_app.hpp"
#include "state/rocket_state.hpp"
#include "state/state_update.hpp"
#include "hud/draw/hud_draw.hpp"

int main() {

    auto app = SetupHudApp();
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);

    while (!WindowShouldClose()) {

        UpdateState(app, app.runningData, serial);
        if (app.state.stage != FlightStage::Calibrating) {
            UpdateShader(app.camera, app.shader, app.light);
        }
        DrawHud(app);

    }

    app.unload();
}
