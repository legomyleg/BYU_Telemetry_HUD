#pragma once
#include <opencv2/opencv.hpp>
#include <raylib.h>
#include <hud/rlights.h>
#include <hud/hud_layout.hpp>
#include <telemetry/sample_buffer.hpp>
#include <state/rocket_state.hpp>
#include <hud/config.hpp>
#include <telemetry/serial.hpp>
#include <telemetry/telemetry_stats.hpp>
#include <hud/colors.hpp>
#include <state/calibration.hpp>
#include <cstdint>
#include <string>
#include <vector>
using std::vector;
using std::string;

struct AltAtT {
    float alt_m;
    float time_s;
};

enum class UpdateMode {
    Normal,
    ReSync
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
    SampleQueue sample_queue;
    uint64_t last_measured_time;
    RocketState state;
    TelemetryStats telemetry;
    vector<AltAtT> measuredAlts;
    bool camera_feed_enabled = false;
    cv::Mat curr_frame;
    Texture2D frame_texture{};

    string data_buffer;
    UpdateMode updateMode = UpdateMode::Normal;
    vector<SensorData> resyncArray{};

    void unload();

    HudApp(uint64_t buffer_size) : state(buffer_size) {}
};

HudApp SetupHudApp(uint64_t buffer_size);
void UpdateShader(Camera3D camera, Shader shader, Light light);
