#include "hud_draw.hpp"
#include <raylib.h>
#include <string>
#include <format>
#include <iterator>
#include "hud/config.hpp"
#include "telemetry/telemetry_config.hpp"
#include "hud/hud_app.hpp"
#include "hud/milestones.hpp"
using std::string;
using std::format;


struct WrittenText {
    string title;
    vector<string> lines;
};

string roundedStr(float val) {
    return format("{:.2f}", val);
}

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, const Color color) {
    Vector2 size = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

void DrawTextCenteredToTop(Font font, const char *text, Rectangle rect, float fontSize, const Color color, float margin) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + margin
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

void DrawTextCenteredAtY(Font font, const char* text, Rectangle rect, float fontSize, const Color color, float y) {
    // Y val is absolute, relative to the screen, NOT the rect

    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
            rect.x + (rect.width - textSize.x) / 2,
            y
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

void DrawTextCenteredAtXVertical(Font font, const char* text, Rectangle rect, float fontSize, const Color color, float x) {
    // X val is relative to screen, NOT rect
    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
            x,
            rect.y + (rect.height + textSize.x) / 2
    };

    DrawTextPro(font, text, position, Vector2{0,0}, -90.0f, fontSize, TEXT_SPACING, color);
}

void DrawFieldsInBox(Font font, Rectangle& box, WrittenText &text, const ColorPalette &colors, float gapSize) {
    DrawTextCenteredToTop(font, text.title.c_str(), box, TEXT_SIZE, colors.headerTextColor, 0);

    Vector2 nextPos = {box.x, box.y + TEXT_SIZE + gapSize};
    for (const auto & line : text.lines) {
        DrawTextEx(font, line.c_str(), nextPos, TEXT_SIZE, TEXT_SPACING, colors.textColor);
        nextPos.y += TEXT_SIZE + gapSize;
    }
}

void DrawBackground(const HudBox &box, const ColorPalette &colors) {
    DrawRectangleRec(box, colors.panelBackgroundColor);
}

struct Bar {
    Vector2 startPos = {0,0};
    Vector2 endPos = {0,0};
    float currentAlt = 0;
    int halfBarWidth = 5;

    void drawBar() {
        DrawLine(startPos.x, startPos.y, endPos.x, getYPos(currentAlt), GRAY);
        DrawLine(startPos.x, getYPos(currentAlt), endPos.x, endPos.y, WHITE);

        DrawLine(startPos.x - halfBarWidth, startPos.y, startPos.x + halfBarWidth, startPos.y, GRAY);

        Color topBarColor = (currentAlt * M2FT) >= 30000.0f ? GRAY : WHITE;
        DrawLine(endPos.x - halfBarWidth, endPos.y, endPos.x + halfBarWidth, endPos.y, topBarColor);
    }

    float length() {
        return startPos.y - endPos.y;
    }

    float getYPos(float altM) {
        float pixelsPerM = length() / (ALT_BAR_HEIGHT_FT * FT2M); // Converting to meters
        return startPos.y - (altM * pixelsPerM);
    }

    void writeSideTextAt(Font font, string text, float fontSize, float altM) {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, TEXT_SPACING);
        Vector2 position = {
                startPos.x - halfBarWidth - 5.0f - textSize.x,
                getYPos(altM) - (textSize.y / 2.0f)
        };

        Color color = currentAlt > altM ? GREEN : WHITE;
        DrawTextEx(font, text.c_str(), position, fontSize, TEXT_SPACING, color);
    }

    void drawMark(float altM) {
        Color color = currentAlt > altM ? GRAY : WHITE;
        float yPos = getYPos(altM);
        DrawLine(startPos.x - halfBarWidth, yPos, startPos.x + halfBarWidth, yPos, color);
    }

    void draw(Font font, float fontSize) {
        drawBar();
        writeSideTextAt(font, ALT_BAR_TOP_HEIGHT_TEXT, fontSize, (ALT_BAR_HEIGHT_FT * FT2M));
        for (const auto &ms : MILESTONES) {
            drawMark(ms.altitude_m);
            writeSideTextAt(font, ms.name, fontSize, ms.altitude_m);
        }

        DrawCircle(startPos.x, getYPos(currentAlt), 4, RED);
    }

};

void DrawAltBar(const HudBox &box, float currentAlt, Font font) {
    int marginToSide = 15;
    int marginToTop = 10;

    Bar bar;
    bar.startPos = {
            box.x + box.width - marginToSide,
            box.y + box.height - marginToTop
    };
    bar.endPos = {
            bar.startPos.x,
            box.y + marginToTop
    };
    bar.currentAlt = currentAlt;
    bar.draw(font, TEXT_SIZE);
}

void DrawSceneBox(const HudBox &box, 
    const RenderTexture2D &sceneTarget,
    const Camera3D &camera, 
    const Model &rocket,
    const ColorPalette &colors,
    float currentAlt,
    Font font
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
        box,
        {0, 0},
        0.0f,
        WHITE
    );

    DrawAltBar(box, currentAlt, font);
}

void DrawCameraFeedBox(Font font, const HudBox &box, const ColorPalette &colors) {
    DrawRectangleRec(box, {0,0,0,255});

    DrawTextCenteredEx(font, "CAMERA FEED", box, 35, colors.headerTextColor);
}

void DrawStageIndicator(Vector2 center, float radius, const StageInfo &si, bool active, Font font) {
    Color color = active ? GREEN : GRAY;

    DrawCircleV(center, radius, color);

    float fontSize = 15.0f;
    Vector2 textSize = MeasureTextEx(font, si.label, fontSize, TEXT_SPACING);
    Vector2 textPosition = {
            center.x - (textSize.x / 2),
            center.y + (radius + 10.0f)
    };

    DrawTextEx(font, si.label, textPosition, fontSize, TEXT_SPACING, WHITE);
}

void DrawStagesBox(Font font, const HudBox &box, const ColorPalette &colors, FlightStage stage) {

    float radius = 30.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "STAGE", box, 15.0f, colors.headerTextColor, 10);

    if (stage == FlightStage::Calibrating) {
        DrawTextCenteredEx(font, "CALIBRATING", box, 30.0f, WHITE);
        return;
    }

    size_t stageCount = std::size(STAGES);
    float totalCircleWidth = 2.0f * radius * (float)stageCount;
    float gap = (box.width - totalCircleWidth) / ((float)stageCount + 1.0f);
    float posY = box.y + (box.height / 2.0f);
    for (int i=0; i < stageCount; i++) {
        int ng = i + 1;
        int nr = 2*i + 1;
        float posX = box.x + ((float)ng*gap + (float)nr*radius);

        Vector2 center = {posX, posY};

        bool active = (stage == STAGES[i].stage);

        DrawStageIndicator(center, radius, STAGES[i], active, font);
    }
}

void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state) {
    float margin = 15.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "SENSOR DATA", box, 15.0f, colors.headerTextColor, margin);

    float textBoxHeight = box.height - HEADER_SIZE - (margin * 3);
    float textBoxWidth = (box.width - (margin * 4)) / 3.0f;
    float textBoxY = box.y + HEADER_SIZE + margin*2;

    Rectangle velocity = {box.x + margin, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle attitude = {box.x + (margin*2) + textBoxWidth, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle altitude = {box.x + margin*3 + textBoxWidth*2, textBoxY, textBoxWidth, textBoxHeight};

    // Ttrying out drawing individual boxes
    DrawRectangleRoundedLines(velocity,BOX_ROUNDNESS,8.0f, colors.screenDividerColor);
    // Ending this

    string velXyz = "X: " + roundedStr(state.velocity.x) +
            " Y: " + roundedStr(state.velocity.y) +
            " Z: " + roundedStr(state.velocity.z);
    string totalVel = "Total Velocity: " + roundedStr(state.total_velocity) + "m/s";
    string vertVel = "Vertical Velocity: " + roundedStr(state.vertical_velocity_mps) + "m/s";
    vector<string> vDataVals = {velXyz, totalVel, vertVel};
    WrittenText velText = {"VELOCITY", vDataVals};

    string attRoll = "Roll: " + roundedStr(state.attitude.roll);
    string attPitch = "Pitch: " + roundedStr(state.attitude.pitch);
    string attYaw = "Yaw: " + roundedStr(state.attitude.yaw);
    vector<string> attDataVals = {attRoll, attPitch, attYaw};
    WrittenText attText = {"ATTITUDE", attDataVals};

    string altVal = "Altitude: " + roundedStr(state.altitude);
    WrittenText altText = {"ALTITUDE", {altVal}};

    DrawFieldsInBox(font, velocity, velText, colors, 10.0f);
    DrawFieldsInBox(font, attitude, attText, colors, 10.0f);
    DrawFieldsInBox(font, altitude, altText, colors, 10.0f);
}

void DrawGraphBox(Font font, const HudBox &box, const ColorPalette &colors, const vector<AltAtT> &measuredAlts) {

    float margin = TEXT_SIZE + 10.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "GRAPH", box, 15.0f, colors.headerTextColor, 10.0f);

    Vector2 cornerPoint = {box.x + margin, box.y + (box.height - margin)};
    Vector2 yAxisEndpoint = {cornerPoint.x, box.y + margin};
    Vector2 xAxisEndpoint = {box.x + (box.width - margin), cornerPoint.y};

    DrawLine((int)cornerPoint.x, (int)cornerPoint.y, (int)yAxisEndpoint.x, (int)yAxisEndpoint.y, WHITE);
    DrawLine((int)cornerPoint.x, (int)cornerPoint.y, (int)xAxisEndpoint.x, (int)xAxisEndpoint.y, WHITE);
    DrawTextCenteredAtY(font, "TIME (10s)", box, TEXT_SIZE, WHITE, cornerPoint.y + 5.0f);
    DrawTextCenteredAtXVertical(font, "ALTITUDE (500m)", box, TEXT_SIZE, WHITE, box.x + 5.0f);

    int xAxisNotches = EXPECTED_FLIGHT_TIME_S / 10; // 1 notch = 10sec
    int xAxisNotchGap = (int)(xAxisEndpoint.x - cornerPoint.x) / (xAxisNotches + 1);
    float pixelsPerSec = 10.0f / (float)xAxisNotchGap;
    for (int i=1; i <= xAxisNotches; i++) {
        int xLoc = (int)cornerPoint.x + (i * xAxisNotchGap);
        DrawLine(xLoc, (int)cornerPoint.y, xLoc, (int)cornerPoint.y - 5, WHITE);
    }

    int yAxisNotches = GRAPH_ALT_HEIGHT_M / 500; // 1 notch = 500m
    int yAxisNotchGap = (int)(cornerPoint.y - yAxisEndpoint.y) / (yAxisNotches + 1);
    float pixelsPerM = 500.0f / (float)yAxisNotchGap;
    for (int i=1; i <= yAxisNotches; i++) {
        int yLoc = (int)cornerPoint.y - (i * yAxisNotchGap);
        DrawLine((int)cornerPoint.x, yLoc, (int)cornerPoint.x + 5, yLoc, WHITE);
    }

    for (const auto measuredAlt : measuredAlts) {
        Vector2 position = {
                cornerPoint.x + (pixelsPerSec * measuredAlt.time_s),
                cornerPoint.y - (pixelsPerM * measuredAlt.alt_m)
        };
        DrawCircleV(position, 3.0f, RED);
    }
}

void DrawTelemetryBox(const HudApp &app) {

    HudBox box = app.layout.telem;
    float margin = 10.0f;
    
    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, app.colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, app.colors.panelBorderColor);
    DrawTextCenteredToTop(app.hudFont, "TELEMETRY STATS", box, HEADER_SIZE, app.colors.headerTextColor, margin);

    // ADD TELEMETRY STATS HERE

    // Create info boxes
    float titleHeight = margin + HEADER_SIZE;
    float remHeight = box.height - titleHeight;
    float textBoxHeight = (box.height - (margin * 4.0f)) / 3.0f;
    float textBoxWidth = box.width - (margin * 2.0f);
    float xVal = box.x + margin;

    float spsY = box.y + titleHeight + margin;
    Rectangle spsBox = {xVal, spsY, textBoxWidth, textBoxHeight};

    float pdY = spsY + textBoxHeight + margin;
    Rectangle pdBox = {xVal, pdY, textBoxWidth, textBoxHeight};

    float ukY = pdY + textBoxHeight + margin;
    Rectangle ukBox = {xVal, ukY, textBoxWidth, textBoxHeight};

    // SAMPLES RECEIVED PER SECOND
    float sps = app.telemetry.samples_per_sec;
    WrittenText spsText = {"SAMPLES PER SECOND", {roundedStr(sps)}};

    // PACKETS DROPPED
    float pd = app.telemetry.packets_dropped;
    WrittenText pdText = {"PACKETS DROPPED", {roundedStr(pd)}};

    float uk = 0;
    WrittenText ukText = {"TBD Value", {roundedStr(uk)}};

    DrawFieldsInBox(app.hudFont, spsBox, spsText, app.colors, 10.0f);
    DrawFieldsInBox(app.hudFont, pdBox, pdText, app.colors, 10.0f);
    DrawFieldsInBox(app.hudFont, ukBox, ukText, app.colors, 10.0f);
    
    // RUNTIME?
}

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const ColorPalette &colors) {
    DrawLine((int)sceneWidth, 0, (int)sceneWidth, (int)screenHeight, colors.screenDividerColor);
    DrawLine(0, (int)sceneHeight, (int)sceneWidth, (int)sceneHeight, colors.screenDividerColor);
}

void DrawHud(const HudApp &app){

    BeginDrawing();

    DrawSceneBox(app.layout.scene, app.sceneTarget, app.camera, app.rocket, app.colors, app.state.altitude, app.hudFont);
    DrawCameraFeedBox(app.hudFont, app.layout.cameraFeed, app.colors);
    DrawDivider(app.layout.scene.height, app.layout.scene.width, app.layout.screenHeight, app.colors);
    DrawBackground(app.layout.panelBackground, app.colors);
    DrawStagesBox(app.hudFont, app.layout.stages, app.colors, FlightStage::Boost);
    DrawSensorsBox(app.hudFont, app.layout.sensors, app.colors, app.state);
    DrawGraphBox(app.hudFont, app.layout.graph, app.colors, app.measuredAlts);
    DrawTelemetryBox(app);

    EndDrawing();
}
