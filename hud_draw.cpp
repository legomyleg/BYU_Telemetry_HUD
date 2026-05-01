#include "hud_draw.hpp"

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, Color color) {
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2
    };

    DrawTextEx(font, text, position, fontSize, spacing, color);
}

void DrawTextCenteredToTop(Font font, const char *text, Rectangle rect, float fontSize, float spacing, Color color, float margin) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + margin
    };

    DrawTextEx(font, text, position, fontSize, spacing, color);
}

void DrawBackground(const HudBox &box, const Color color) {
    DrawRectangleRec(box.bounds, color);
}

void DrawSceneBox(const HudBox &box, 
    RenderTexture2D &sceneTarget, 
    const Camera3D &camera, 
    const Model &rocket,
    const Color backgroundColor
) {
    BeginTextureMode(sceneTarget);
    ClearBackground(backgroundColor);

    BeginMode3D(camera);
    DrawModel(rocket, {0, 0, 0}, 0.3f, WHITE);
    EndMode3D();

    EndTextureMode();

    DrawTexturePro(
        sceneTarget.texture,
        {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height},
        box.bounds,
        {0, 0},
        0.0f,
        WHITE
    );
}

void DrawCameraFeedBox(Font font, const HudBox &box) {
    DrawRectangleRec(box.bounds, {0,0,0,255});

    DrawTextCenteredEx(font, "CAMERA FEED", box.bounds, 35, TEXT_SPACING, WHITE);
}

void DrawStagesBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, panelBorderColor);
    DrawTextCenteredToTop(font, "STAGE", box.bounds, 15.0f, TEXT_SPACING, GREEN, 10);

    // ADD STAGING PARTS HERE

}

void DrawSensorsBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor, const SensorData& data) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, panelBorderColor);
    DrawTextCenteredToTop(font, "SENSOR DATA", box.bounds, 15.0f, TEXT_SPACING, GREEN, 10.0f);

    // ADD SENSOR DATA HERE
}

void DrawGraphBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, panelBorderColor);
    DrawTextCenteredToTop(font, "GRAPH", box.bounds, 15.0f, TEXT_SPACING, GREEN, 10.0f);

    // ADD GRAPH HERE
}

void DrawReceivingBox(Font font, const HudBox &box, const Color panelColor, const Color panelBorderColor) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, panelBorderColor);
    DrawTextCenteredToTop(font, "TELEMETRY STATS", box.bounds, 15.0f, TEXT_SPACING, GREEN, 10.0f);

    // ADD TELEMETRY STATS HERE
}

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const Color color) {
    DrawLine(sceneWidth, 0, sceneWidth, screenHeight, color);
    DrawLine(0, sceneHeight, sceneWidth, sceneHeight, color);
}

void DrawHud(const HudLayout &layout, Font font, const ColorPalette colors, const RenderingData &rendering, const SensorData& data){

    BeginDrawing();

    DrawSceneBox(layout.scene, *rendering.sceneTarget, *rendering.camera, *rendering.rocket, colors.sceneBackgroundColor);
    DrawCameraFeedBox(font, layout.cameraFeed);
    DrawDivider(layout.scene.bounds.height, layout.scene.bounds.width, layout.screenHeight, colors.screenDividerColor);
    DrawBackground(layout.panelBackground, colors.panelBackgroundColor);
    DrawStagesBox(font, layout.stages, colors.panelColor, colors.panelBorderColor);
    DrawSensorsBox(font, layout.sensors, colors.panelColor, colors.panelBorderColor, data);
    DrawGraphBox(font, layout.graph, colors.panelColor, colors.panelBorderColor);
    DrawReceivingBox(font, layout.receiving, colors.panelColor, colors.panelBorderColor);

    EndDrawing();
}
