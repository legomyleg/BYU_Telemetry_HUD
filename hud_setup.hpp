#pragma once
#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "hud_layout.hpp"
#include "types.hpp"
#include "config.hpp"
#include "serial.hpp"
#include <string>
using std::string;


struct HudApp {
    Font hudFont;
    Model rocket;
    Shader shader;
    Light light;
    Camera3D camera;
    RenderTexture2D sceneTarget;
    HudLayout layout;
    SampleBuffer running_data;
    uint64_t last_measured_time;
    Quaternion orientation;

    string serial_buffer;

    void unload() {
        UnloadFont(hudFont);
        UnloadModel(rocket);
        UnloadShader(shader);
        UnloadRenderTexture(sceneTarget);
    }
};

inline HudApp SetupHudApp() {
    HudApp app{};

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rocket Attitude Visualizer");
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(SAMPLE_RATE);

    app.hudFont = LoadFontEx("resources/fonts/HelveticaNeueRoman.otf", 32, nullptr, 0);
    SetTextureFilter(app.hudFont.texture, TEXTURE_FILTER_BILINEAR);

    app.rocket = LoadModel(MODEL_PATH);

    app.shader = LoadShader(
    "resources/shaders/glsl330/lighting.vs",
    "resources/shaders/glsl330/lighting.fs"
    );

    app.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(app.shader, "viewPos");

    for (int i = 0; i < app.rocket.materialCount; i++) {
        app.rocket.materials[i].shader = app.shader;
    }

    int ambientLoc = GetShaderLocation(app.shader, "ambient");

    float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    SetShaderValue(app.shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    app.light = CreateLight(LIGHT_POINT, {5.0f, 10.0f, 5.0f}, Vector3Zero(), WHITE, app.shader);

    app.camera = {0};
    app.camera.position = {0.0f, 5.0f, 10.0f};
    app.camera.target = {0.0f, 0.0f, 0.0f};
    app.camera.up         = {0.0f, 1.0f, 0.0f};
    app.camera.fovy       = 45.0f;
    app.camera.projection = CAMERA_PERSPECTIVE;

    app.sceneTarget = LoadRenderTexture(SCENE_WIDTH * SCENE_RENDER_SCALE, 
        SCENE_HEIGHT * SCENE_RENDER_SCALE);
    SetTextureFilter(app.sceneTarget.texture, TEXTURE_FILTER_BILINEAR);

    app.layout = MakeHudLayout(SCREEN_WIDTH, SCREEN_HEIGHT);

    app.orientation = QuaternionIdentity();
    app.last_measured_time = 0;

    return app;
}

