#include "raylib.h"
#include "raymath.h"
#include "tools.hpp"
#include "colors.hpp"
#include "hud_draw.hpp"
#include "config.hpp"
#include "hud_setup.hpp"
#include "state_update.hpp"

const auto *colors = &WARMER_SPACE_PALETTE_1;

int main() {

    auto app = SetupHudApp();
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", B9600);
    uint64_t last_measured_time;
    float dt_s;

    // Initialize the time
    ReadSerialSamples(app, app.running_data, serial);
    SensorData latest_sample = app.running_data.consume_latest();
    app.last_measured_time = latest_sample.t_us;

    while (!WindowShouldClose()) {

        ReadSerialSamples(app, app.running_data, serial);
        if (!app.running_data.empty()) {
            latest_sample = app.running_data.latest();
        }
        UpdateState(app, app.running_data);

        float camPos[3] = {app.camera.position.x, app.camera.position.y, app.camera.position.z};
        SetShaderValue(app.shader, app.shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(app.shader, app.light);

        // --- DRAW ---
        auto rendering = RenderingData(app.sceneTarget, app.camera, app.rocket);
        DrawHud(app.layout, app.hudFont, *colors, rendering, latest_sample);

    }

    app.unload();
}
