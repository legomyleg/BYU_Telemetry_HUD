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
#include <chrono>
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
    SampleBuffer sample_buffer;
    uint64_t last_measured_time;
    RocketState state;
    TelemetryStats telemetry;
    Biases biases;
    vector<AltAtT> measuredAlts;

    string data_buffer;

    void unload();

    HudApp(std::chrono::microseconds buffer_size) : state(buffer_size) {}
};

HudApp SetupHudApp(std::chrono::microseconds buffer_size);
void UpdateShader(Camera3D camera, Shader shader, Light light);
