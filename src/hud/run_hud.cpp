#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <hud/draw/hud_draw.hpp>
#include <state/state_update.hpp>
#include <raylib.h>
#include <hud/draw/home_window.hpp>
#include <state/rocket_state.hpp>

void RunHud(TelemetrySource *data_src, uint64_t buffer_size, bool no_calibrate) {
    bool initialized = false;

    auto app = SetupHudApp(buffer_size);

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

            UpdateState(app, app.sample_queue, *data_src);
            if (app.state.stage != FlightStage::Calibrating) {
                UpdateShader(app.camera, app.shader, app.light);
            }
        }

        DrawHud(app);

        EndDrawing();
    }

    app.unload();

}
