#pragma once
#include "raylib.h"

inline constexpr float SCENE_WIDTH_RATIO = 2.0f / 3.0f;
inline constexpr float STAGES_HEIGHT_RATIO = 0.25f;
inline constexpr float SENSORS_HEIGHT_RATIO = 0.5f - STAGES_HEIGHT_RATIO;
inline constexpr float BOTTOM_PANELS_HEIGHT_RATIO = 1.0f - (SENSORS_HEIGHT_RATIO + STAGES_HEIGHT_RATIO);
inline constexpr float GRAPH_WIDTH_RATIO = 0.5f;

struct HudBox {
    Rectangle bounds;
};

struct HudLayout {
    HudBox scene;
    HudBox cameraFeed;
    HudBox stages;
    HudBox sensors;
    HudBox graph;
    HudBox receiving;
};

inline HudLayout MakeHudLayout(int screenWidth, int screenHeight) {
    const float sceneWidth = screenWidth * SCENE_WIDTH_RATIO;
    const float sceneHeight = screenHeight / 2;
    const float cameraFeedHeight = screenHeight - sceneHeight;
    const float cameraFeedWidth = sceneWidth;

    const float panelsWidth = screenWidth - sceneWidth;

    const float margin = 10.0f;
    const float doubleMargin = 2 * margin;
    const float halfMargin = margin / 2;
    
    HudLayout layout;

    layout.scene.bounds = {0, 0, sceneWidth, sceneHeight};
    layout.cameraFeed.bounds = {0, sceneHeight, cameraFeedWidth, cameraFeedHeight};

    const float stagesHeight = screenHeight * STAGES_HEIGHT_RATIO;
    const float sensorsHeight = screenHeight * SENSORS_HEIGHT_RATIO;
    const float graphHeight = screenHeight * BOTTOM_PANELS_HEIGHT_RATIO;
    const float graphWidth = (screenWidth - sceneWidth) * GRAPH_WIDTH_RATIO;
    const float receivingWidth = screenWidth - (sceneWidth + graphWidth);
    const float receivingHeight = graphHeight;

    layout.stages.bounds = {sceneWidth + margin, margin, panelsWidth - doubleMargin, stagesHeight - (margin + halfMargin)};
    layout.sensors.bounds = {sceneWidth + margin, stagesHeight + halfMargin, panelsWidth - doubleMargin, sensorsHeight - margin};
    layout.graph.bounds = {sceneWidth + margin, stagesHeight + sensorsHeight + halfMargin, graphWidth - (margin + halfMargin), graphHeight - (margin + halfMargin)};
    layout.receiving.bounds = {sceneWidth + graphWidth + halfMargin, stagesHeight + sensorsHeight + halfMargin, receivingWidth - (margin + halfMargin), receivingHeight - (margin + halfMargin)};

    return layout;
}

