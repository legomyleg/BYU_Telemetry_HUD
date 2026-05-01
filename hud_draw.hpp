#pragma once

#include "raylib.h"
#include "hud_layout.hpp"
#include "types.hpp"
#include "colors.hpp"
#include "config.hpp"
#include "hud_setup.hpp"

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

void DrawTextCenteredToTop(Font font, const char *text, Rectangle rect, float fontSize, float spacing, const ColorPalette &colors, float margin);

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, const ColorPalette &colors);

void DrawFieldsInBox(Font font, Rectangle& box, WrittenText &text, const ColorPalette &colors, float gapSize);

void DrawBackground(const HudBox &box, const ColorPalette &colors);

void DrawSceneBox(const HudBox &box, 
    const RenderTexture2D &sceneTarget,
    const Camera3D &camera, 
    const Model &rocket,
    const ColorPalette &colors
);

void DrawCameraFeedBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawStagesBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state);

void DrawGraphBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawReceivingBox(Font font, const HudBox &box, const ColorPalette &colors);

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const ColorPalette &colors);

void DrawHud(const HudApp &app);
