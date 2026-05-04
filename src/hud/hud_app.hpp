#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlights.h"
#include "hud_layout.hpp"
#include "telemetry/sample_buffer.hpp"
#include "state/rocket_state.hpp"
#include "config.hpp"
#include "telemetry/serial.hpp"
#include "colors.hpp"
#include <string>
using std::string;


struct HudApp {
    Font hudFont;
    ColorPalette colors = COLOR_PALETTE;
    Model rocket;
    Shader shader;
    Light light;
    Camera3D camera;
    RenderTexture2D sceneTarget;
    HudLayout layout;
    SampleBuffer running_data;
    uint64_t last_measured_time;
    RocketState state;

    string serial_buffer;

    void unload();
};

HudApp SetupHudApp();
