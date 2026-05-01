#include "hud_draw.hpp"
#include <string>
using std::string, std::to_string;

struct WrittenText {
    string title;
    vector<string> lines;
};

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, const ColorPalette &colors) {
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2
    };

    DrawTextEx(font, text, position, fontSize, spacing, colors.textColor);
}

void DrawTextCenteredToTop(Font font, const char *text, Rectangle rect, float fontSize, float spacing, const ColorPalette &colors, float margin) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + margin
    };

    DrawTextEx(font, text, position, fontSize, spacing, colors.headerTextColor);
}

void DrawFieldsInBox(Font font, Rectangle& box, WrittenText &text, const ColorPalette &colors, float gapSize) {
    DrawTextCenteredToTop(font, text.title.c_str(), box, TEXT_SIZE, TEXT_SPACING, colors, 0);

    Vector2 nextPos = {box.x, TEXT_SIZE + gapSize};
    int i = 0;
    while (!text.lines.empty()) {
        DrawTextEx(font, text.lines[i++].c_str(), nextPos, TEXT_SIZE, TEXT_SPACING, colors.textColor);
        nextPos.y += TEXT_SIZE + gapSize;
    }
}

void DrawBackground(const HudBox &box, const ColorPalette &colors) {
    DrawRectangleRec(box.bounds, colors.panelBackgroundColor);
}

void DrawSceneBox(const HudBox &box, 
    RenderTexture2D &sceneTarget, 
    const Camera3D &camera, 
    const Model &rocket,
    const ColorPalette &colors
) {
    BeginTextureMode(sceneTarget);
    ClearBackground(colors.sceneBackgroundColor);

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

void DrawCameraFeedBox(Font font, const HudBox &box, const ColorPalette &colors) {
    DrawRectangleRec(box.bounds, {0,0,0,255});

    DrawTextCenteredEx(font, "CAMERA FEED", box.bounds, 35, TEXT_SPACING, colors);
}

void DrawStagesBox(Font font, const HudBox &box, const ColorPalette &colors) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "STAGE", box.bounds, 15.0f, TEXT_SPACING, colors, 10);

    // ADD STAGING PARTS HERE

}

void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state) {
    float margin = 15.0f;

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "SENSOR DATA", box.bounds, 15.0f, TEXT_SPACING, colors, margin);

    float textBoxHeight = box.bounds.height - HEADER_SIZE - (margin * 3);
    float textBoxWidth = box.bounds.width - (margin * 4);
    float textBoxY = box.bounds.y + HEADER_SIZE + margin*2;

    Rectangle velocity = {box.bounds.x + margin, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle attitude = {box.bounds.x + margin*2 + textBoxWidth, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle altitude = {box.bounds.x + margin*3 + textBoxWidth*2, textBoxY, textBoxWidth, textBoxHeight};

    string velXyz = "X: " + to_string(state.velocity.x) +
            " Y: " + to_string(state.velocity.y) +
            " Z: " + to_string(state.velocity.z);
    string totalVel = "Total Velocity: " + to_string(state.total_velocity) + "m/s";
    string vertVel = "Vertical Velocity: " + to_string(state.vertical_velocity_mps) + "m/s";
    vector<string> vDataVals = {velXyz, totalVel, vertVel};
    WrittenText velText = {"VELOCITY", vDataVals};

    string attRoll = "Roll: " + to_string(state.attitude.roll);
    string attPitch = "Pitch: " + to_string(state.attitude.pitch);
    string attYaw = "Yaw: " + to_string(state.attitude.yaw);
    vector<string> attDataVals = {attRoll, attPitch, attYaw};
    WrittenText attText = {"ATTITUDE", attDataVals};

    string altitude = "Altitude: " + to_string(state.altitude);
    WrittenText altText = {"ALTITUDE", {altitude}};

    DrawFieldsInBox(font, velocity, velText, colors, 10.0f);
    DrawFieldsInBox(font, attitude, attText, colors, 10.0f);
    DrawFieldsInBox(font, altitude, altText, colors, 10.0f);
}

void DrawGraphBox(Font font, const HudBox &box, const ColorPalette &colors) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "GRAPH", box.bounds, 15.0f, TEXT_SPACING, colors, 10.0f);

    // ADD GRAPH HERE
}

void DrawReceivingBox(Font font, const HudBox &box, const ColorPalette &colors) {

    DrawRectangleRounded(box.bounds, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box.bounds, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "TELEMETRY STATS", box.bounds, 15.0f, TEXT_SPACING, colors, 10.0f);

    // ADD TELEMETRY STATS HERE
}

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const ColorPalette &colors) {
    DrawLine(sceneWidth, 0, sceneWidth, screenHeight, colors.screenDividerColor);
    DrawLine(0, sceneHeight, sceneWidth, sceneHeight, colors.screenDividerColor);
}

void DrawHud(const HudLayout &layout, Font font, const ColorPalette &colors, const RenderingData &rendering, const RocketState& stateData){

    BeginDrawing();

    DrawSceneBox(layout.scene, *rendering.sceneTarget, *rendering.camera, *rendering.rocket, colors);
    DrawCameraFeedBox(font, layout.cameraFeed, colors);
    DrawDivider(layout.scene.bounds.height, layout.scene.bounds.width, layout.screenHeight, colors);
    DrawBackground(layout.panelBackground, colors);
    DrawStagesBox(font, layout.stages, colors);
    DrawSensorsBox(font, layout.sensors, colors, data);
    DrawGraphBox(font, layout.graph, colors);
    DrawReceivingBox(font, layout.receiving, colors);

    EndDrawing();
}
