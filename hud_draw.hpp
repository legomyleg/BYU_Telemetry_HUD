#pragma once

#include "raylib.h"
#include "hud_layout.hpp"
#include "types.hpp"
#include "colors.hpp"

inline constexpr float TEXT_SPACING = 2.0f;
inline constexpr float BOX_ROUNDNESS = 0.08f;

struct RenderingData {
    RenderTexture2D *sceneTarget = nullptr;
    const Camera3D *camera = nullptr;
    const Model *rocket = nullptr;

    RenderingData() = default;
    RenderingData(RenderTexture2D& _sceneTarget, Camera3D& _camera, Model& _rocket) : 
    sceneTarget(&_sceneTarget), camera(&_camera), rocket(&_rocket) {}
};

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, Color color);

void DrawBackground(const HudBox &box, const Color color);

void DrawSceneBox(const HudBox &box, 
    RenderTexture2D &sceneTarget, 
    const Camera3D &camera, 
    const Model &rocket,
    const Color backgroundColor
);

void DrawCameraFeedBox(Font font, const HudBox &box);

void DrawStagesBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor);

void DrawSensorsBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor, const SensorData& data);

void DrawGraphBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor);

void DrawReceivingBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor);

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const Color color);

void DrawHud(const HudLayout &layout, Font font, const ColorPalette colors, const RenderingData &rendering, const SensorData& data);