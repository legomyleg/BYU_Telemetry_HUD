#pragma once

#include "raylib.h"
#include "hud_layout.hpp"
#include "types.hpp"
#include "colors.hpp"

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, Color color);

void DrawSceneBox(const HudBox &box, 
    RenderTexture2D &sceneTarget, 
    const Camera3D &camera, 
    const Model &rocket,
    const Color backgroundColor
);

void DrawCameraFeedBox(Font font, const HudBox &box, const Color dividerColor);

void DrawStagesBox(Font font, const HudBox &box);

void DrawSensorsBox(Font font, const HudBox &box);

void DrawGraphBox(Font font, const HudBox &box);

void DrawReceivingBox(Font font, const HudBox &box);