#include "hud/draw/utils.hpp"
#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <hud/draw/hud_draw.hpp>
#include <memory>
#include <optional>
#include <state/state_update.hpp>
#include <raylib.h>
#include <hud/draw/home_window.hpp>
#include <state/rocket_state.hpp>
#include <telemetry/feed/rtsp_receiver.hpp>

void RunHud(TelemetrySource *data_src, uint64_t buffer_size, bool no_calibrate, bool get_feed, std::optional<CameraFeedConfig> config) {
    bool initialized = false;

    auto app = SetupHudApp(buffer_size);

    if (DATA_LOGGING) {
        app.logger.emplace();
    }

    std::unique_ptr<CameraFeed> feed;
    if (config.has_value()) {
        app.camera_feed_enabled = true;
        feed = std::make_unique<CameraFeed>(*config);
    }

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
                    data_src->discard_available();
                    initialized = true;
                }
                EndDrawing();
                continue;
            }
            if (feed != nullptr) {
                auto could_get_frame = feed->buffer.getLatest(app.curr_frame);
                if (!could_get_frame) {
                    // std::cerr << "could not get from using getLatest\n";
                } else {
                    UpdateTextureFromMat(app.curr_frame, app.frame_texture);
                }
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
