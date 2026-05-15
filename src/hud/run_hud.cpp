#include "run_hud.hpp"
#include "hud_app.hpp"
#include "draw/hud_draw.hpp"
#include "../state/state_update.hpp"
#include <raylib.h>
#include "draw/home_window.hpp"
#include "state/rocket_state.hpp"

void RunHud(TelemetrySource *data_src, bool no_calibrate) {
    bool initialized = false;

    auto app = SetupHudApp();

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (no_calibrate) {
            app.state.stage = FlightStage::Pad;
        }
        
        if (data_src != nullptr) {
            if (!initialized) {
                Rectangle calib_button = DrawCalibScreen(app);
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos,calib_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    initialized = true;
                }
                EndDrawing();
                continue;
            }

            UpdateState(app, app.runningData, *data_src);
            if (app.state.stage != FlightStage::Calibrating) {
                UpdateShader(app.camera, app.shader, app.light);
            }
        }

        DrawHud(app);

        EndDrawing();
    }

    app.unload();

}
