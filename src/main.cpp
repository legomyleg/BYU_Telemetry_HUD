#include "raylib.h"
#define RLIGHTS_IMPLEMENTATION
#include "hud/rlights.h"
#include "hud/draw/hud_draw.hpp"
#include "hud/hud_app.hpp"
#include "state/state_update.hpp"

int main() {

    auto app = SetupHudApp();
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);

    while (!WindowShouldClose()) {

        UpdateState(app, app.runningData, serial);
        UpdateShader(app.camera, app.shader, app.light);
        DrawHud(app);

    }

    app.unload();
}
