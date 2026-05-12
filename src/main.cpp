#include "raylib.h"
#include "hud/hud_app.hpp"
#include "state/rocket_state.hpp"
#include "state/state_update.hpp"
#include "hud/draw/hud_draw.hpp"
#include "hud/draw/home_window.hpp"

int main() {
    
    bool initialized = false;
    
    auto app = SetupHudApp();
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);

    while (!WindowShouldClose()) {
        
        BeginDrawing();

        if (!initialized) {
            Rectangle calib_button = DrawCalibScreen(app);
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos,calib_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                initialized = true;
            }
            continue;
        }

        UpdateState(app, app.runningData, serial);
        if (app.state.stage != FlightStage::Calibrating) {
            UpdateShader(app.camera, app.shader, app.light);
        }
        DrawHud(app);

        EndDrawing();

    }

    app.unload();
}
