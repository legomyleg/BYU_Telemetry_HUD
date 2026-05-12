#pragma once
#include "raylib.h"
#include "rlights.h"
#include "hud_layout.hpp"
#include "../telemetry/sample_buffer.hpp"
#include "../state/rocket_state.hpp"
#include "config.hpp"
#include "../telemetry/serial.hpp"
#include "../telemetry/telemetry_stats.hpp"
#include "colors.hpp"
#include "state/calibration.hpp"
#include <string>
#include <vector>
using std::vector;
using std::string;

struct AltAtT {
    float alt_m;
    float time_s;
};

struct HudApp {
    Font hudFont;
    ColorPalette colors = COLOR_PALETTE;
    Model rocket;
    Shader shader;
    Light light;
    Camera3D camera;
    RenderTexture2D sceneTarget;
    HudLayout layout;
    SampleBuffer runningData;
    uint64_t lastMeasuredTime;
    RocketState state;
    TelemetryStats telemetry;
    Biases biases;
    vector<AltAtT> measuredAlts;

    string serial_buffer;

    void unload();
};

HudApp SetupHudApp();
void UpdateShader(Camera3D camera, Shader shader, Light light);
