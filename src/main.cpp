#include "raylib.h"
#define RLIGHTS_IMPLEMENTATION
#include "hud/rlights.h"
#include "hud/hud_draw.hpp"
#include "hud/hud_app.hpp"
#include "state/state_update.hpp"

int main() {

    auto app = SetupHudApp();
    SerialPort serial = SerialPort("/dev/cu.usbserial-0001", B9600);

    while (!WindowShouldClose()) {

        UpdateState(app, app.runningData, serial);

        float camPos[3] = {app.camera.position.x, app.camera.position.y, app.camera.position.z};
        SetShaderValue(app.shader, app.shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(app.shader, app.light);

        // --- DRAW ---
        DrawHud(app);

    }

    app.unload();
}
