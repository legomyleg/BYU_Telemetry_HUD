#pragma once
#include "raylib.h"
#include "config.hpp"

using HudBox = Rectangle;

struct HudLayout {
    HudBox scene;
    HudBox cameraFeed;
    HudBox panelBackground;
    HudBox stages;
    HudBox sensors;
    HudBox graph;
    float screenHeight;
    float screenWidth;
};

inline HudLayout MakeHudLayout(int screenWidth, int screenHeight) {
    const float sceneWidth = screenWidth * SCENE_WIDTH_RATIO;
    const float sceneHeight = screenHeight / 2.0f;
    const float cameraFeedHeight = screenHeight - sceneHeight;
    const float cameraFeedWidth = sceneWidth;

    const float panelsWidth = screenWidth - sceneWidth;

    const float margin = 10.0f;
    const float doubleMargin = 2 * margin;
    const float halfMargin = margin / 2;
    
    HudLayout layout;
    layout.screenWidth = screenWidth;
    layout.screenHeight = screenHeight;

    layout.scene = {0, 0, sceneWidth, sceneHeight};
    layout.cameraFeed = {0, sceneHeight, cameraFeedWidth, cameraFeedHeight};


    const float stagesHeight = screenHeight * STAGES_HEIGHT_RATIO;
    const float sensorsHeight = screenHeight * SENSORS_HEIGHT_RATIO;
    const float graphHeight = screenHeight * BOTTOM_PANELS_HEIGHT_RATIO;
    const float graphWidth = (screenWidth - sceneWidth) * GRAPH_WIDTH_RATIO;

    layout.panelBackground = {sceneWidth, 0, panelsWidth, (float)screenHeight};
    layout.stages = {sceneWidth + margin, margin, panelsWidth - doubleMargin, stagesHeight - (margin + halfMargin)};
    layout.sensors = {sceneWidth + margin, stagesHeight + halfMargin, panelsWidth - doubleMargin, sensorsHeight - margin};
    layout.graph = {sceneWidth + margin, stagesHeight + sensorsHeight + halfMargin, graphWidth - (margin + halfMargin), graphHeight - (margin + halfMargin)};

    return layout;
}

