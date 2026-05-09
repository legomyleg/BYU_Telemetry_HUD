#pragma once

#include <string>
#include <vector>
#include "raylib.h"
#include "hud/hud_layout.hpp"
#include "state/rocket_state.hpp"
#include "hud/colors.hpp"
#include "hud/config.hpp"
#include "hud/hud_app.hpp"

using std::string;
using std::vector;

struct RenderingData {
    RenderTexture2D *sceneTarget = nullptr;
    const Camera3D *camera = nullptr;
    const Model *rocket = nullptr;

    RenderingData() = default;
    RenderingData(RenderTexture2D& _sceneTarget, Camera3D& _camera, Model& _rocket) : 
    sceneTarget(&_sceneTarget), camera(&_camera), rocket(&_rocket) {}
};

struct WrittenText {
    string title;
    vector<string> lines;
};

void DrawFieldsInBox(Font font, Rectangle& box, WrittenText &text, const ColorPalette &colors, float gapSize);

void DrawBackground(const HudBox &box, const ColorPalette &colors);

void DrawSceneBox(const HudBox &box, 
    const RenderTexture2D &sceneTarget,
    const Camera3D &camera, 
    const Model &rocket,
    const ColorPalette &colors,
    Font font
);

void DrawCameraFeedBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawStagesBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state);

void DrawGraphBox(Font font, const HudBox &box, const ColorPalette &colors, const vector<AltAtT> &measuredAlts);

void DrawTelemetryBox(const HudApp &app);

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const ColorPalette &colors);

void DrawHud(const HudApp &app);
