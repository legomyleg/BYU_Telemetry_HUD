#include <hud/draw/hud_draw.hpp>
#include <hud/hud_app.hpp>
#include <raylib.h>
#include <string>
#include <iterator>
#include <termios.h>
#include <hud/config.hpp>
#include <telemetry/telemetry_config.hpp>
#include <hud/milestones.hpp>
#include <state/rocket_state.hpp>
#include <hud/draw/utils.hpp>
#include <hud/draw/sensors.hpp>
#include <hud/draw/telem.hpp>
using std::string;

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

    void writeSideTextAt(Font font, string text, float size, float altM) {
        Vector2 textSize = MeasureTextEx(font, text.c_str(), size, TEXT_SPACING);
        Vector2 position = {
                startPos.x - halfBarWidth - 5.0f - textSize.x,
                getYPos(altM) - (textSize.y / 2.0f)
        };

        Color color = currentAlt > altM ? GREEN : WHITE;
        DrawTextEx(font, text.c_str(), position, size, TEXT_SPACING, color);
    }

    void drawMark(float altM) {
        Color color = currentAlt > altM ? GRAY : WHITE;
        float yPos = getYPos(altM);
        DrawLine(startPos.x - halfBarWidth, yPos, startPos.x + halfBarWidth, yPos, color);
    }

    void draw(Font font, float barTextSize) {
        drawBar();
        writeSideTextAt(font, ALT_BAR_TOP_HEIGHT_TEXT, barTextSize, (ALT_BAR_HEIGHT_FT * FT2M));
        for (const auto &ms : MILESTONES) {
            drawMark(ms.altitude_m);
            writeSideTextAt(font, ms.name, barTextSize, ms.altitude_m);
        }

        DrawCircle(startPos.x, getYPos(currentAlt), 4, RED);
    }

};

void DrawAltBar(const HudBox &box, float currentAltAGL, Font font) {
    int marginToSide = 15;
    int marginToTop = 20;

    Bar bar;
    bar.startPos = {
            box.x + box.width - marginToSide,
            box.y + box.height - marginToTop
    };
    bar.endPos = {
            bar.startPos.x,
            box.y + marginToTop
    };
    bar.currentAlt = currentAltAGL;
    bar.draw(font, ALT_BAR_TEXT_SIZE);
}

void DrawSceneBox(const HudBox &box, 
    const RenderTexture2D &sceneTarget,
    const Camera3D &camera, 
    const Model &rocket,
    const ColorPalette &colors,
    float currentAltAGL,
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

    DrawAltBar(box, currentAltAGL, font);
}

void DrawCameraFeedBox(Font font, const HudBox &box, const ColorPalette &colors) {
    DrawRectangleRec(box, {0,0,0,255});

    DrawTextCenteredEx(font, "CAMERA FEED", box, SCREEN_HIJACK_TEXT_SIZE, colors.headerTextColor);
}

void DrawStageIndicator(Vector2 center, float radius, const StageInfo &si, bool active, Font font) {
    Color color = active ? GREEN : GRAY;

    DrawCircleV(center, radius, color);

    Vector2 textSize = MeasureTextEx(font, si.label, STAGE_INDICATOR_TEXT_SIZE, TEXT_SPACING);
    Vector2 textPosition = {
            center.x - (textSize.x / 2),
            center.y + (radius + 10.0f)
    };

    DrawTextEx(font, si.label, textPosition, STAGE_INDICATOR_TEXT_SIZE, TEXT_SPACING, WHITE);
}

void DrawStagesBox(Font font, const HudBox &box, const ColorPalette &colors, FlightStage stage) {

    float radius = 30.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "STAGE", box, BOX_HEADER_SIZE, colors.headerTextColor, 10);

    if (stage == FlightStage::Calibrating) {
        DrawTextCenteredEx(font, "CALIBRATING", box, SCREEN_HIJACK_TEXT_SIZE, WHITE);
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

void DrawGraphBox(Font font, const HudBox &box, const ColorPalette &colors, const vector<AltAtT> &measuredAlts) {

    float margin = GRAPH_AXIS_TEXT_SIZE + 10.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "GRAPH", box, BOX_HEADER_SIZE, colors.headerTextColor, 10.0f);

    Vector2 cornerPoint = {box.x + margin, box.y + (box.height - margin)};
    Vector2 yAxisEndpoint = {cornerPoint.x, box.y + margin};
    Vector2 xAxisEndpoint = {box.x + (box.width - margin), cornerPoint.y};

    DrawLine((int)cornerPoint.x, (int)cornerPoint.y, (int)yAxisEndpoint.x, (int)yAxisEndpoint.y, WHITE);
    DrawLine((int)cornerPoint.x, (int)cornerPoint.y, (int)xAxisEndpoint.x, (int)xAxisEndpoint.y, WHITE);
    DrawTextCenteredAtY(font, "TIME (10s)", box, GRAPH_AXIS_TEXT_SIZE, WHITE, cornerPoint.y + 10.0f);
    DrawTextCenteredAtXVertical(font, "ALTITUDE (500m)", box, GRAPH_AXIS_TEXT_SIZE, WHITE, box.x + 5.0f);

    int xAxisNotches = EXPECTED_FLIGHT_TIME_S / 10; // 1 notch = 10sec
    int xAxisNotchGap = (int)(xAxisEndpoint.x - cornerPoint.x) / (xAxisNotches + 1);
    float pixelsPerSec = (float)xAxisNotchGap / 10.0f;
    for (int i=1; i <= xAxisNotches; i++) {
        int xLoc = (int)cornerPoint.x + (i * xAxisNotchGap);
        DrawLine(xLoc, (int)cornerPoint.y, xLoc, (int)cornerPoint.y - 5, WHITE);
    }

    int yAxisNotches = GRAPH_ALT_HEIGHT_M / 500; // 1 notch = 500m
    int yAxisNotchGap = (int)(cornerPoint.y - yAxisEndpoint.y) / (yAxisNotches + 1);
    float pixelsPerM = (float)yAxisNotchGap / 500.0f;
    for (int i=1; i <= yAxisNotches; i++) {
        int yLoc = (int)cornerPoint.y - (i * yAxisNotchGap);
        DrawLine((int)cornerPoint.x, yLoc, (int)cornerPoint.x + 5, yLoc, WHITE);
    }

    for (const auto measuredAlt : measuredAlts) {
        Vector2 position = {
                cornerPoint.x + (pixelsPerSec * measuredAlt.time_s),
                cornerPoint.y - (pixelsPerM * measuredAlt.alt_m)
        };
        DrawCircleV(position, GRAPH_POINT_RAD, RED);
    }
}

void DrawDivider(const float sceneHeight, const float sceneWidth, const float screenHeight, const ColorPalette &colors) {
    DrawLine((int)sceneWidth, 0, (int)sceneWidth, (int)screenHeight, colors.screenDividerColor);
    DrawLine(0, (int)sceneHeight, (int)sceneWidth, (int)sceneHeight, colors.screenDividerColor);
}

void DrawCalibratingScreen(const HudApp &app) {
    Rectangle screen = {0, 0, app.layout.screenWidth, app.layout.screenHeight};
    DrawRectangleRec(screen, BLACK);

    string text = std::format("Calibrating...{}%", app.state.sample_buffer.percent_full());
    float yPos = screen.height / 2.0f;
    DrawTextCenteredAtY(app.hudFont, text.c_str(), screen, 30.0f, WHITE, yPos);
}

void DrawHud(const HudApp &app){

    if (app.state.stage == FlightStage::Calibrating) {
        DrawCalibratingScreen(app);
        return;
    }

    DrawSceneBox(app.layout.scene, app.sceneTarget, app.camera, app.rocket, app.colors, app.state.altAGL(), app.hudFont);
    DrawCameraFeedBox(app.hudFont, app.layout.cameraFeed, app.colors);
    DrawDivider(app.layout.scene.height, app.layout.scene.width, app.layout.screenHeight, app.colors);
    DrawBackground(app.layout.panelBackground, app.colors);
    DrawStagesBox(app.hudFont, app.layout.stages, app.colors, app.state.stage);
    DrawSensorsBox(app.hudFont, app.layout.sensors, app.colors, app.state);
    DrawGraphBox(app.hudFont, app.layout.graph, app.colors, app.measuredAlts);
    DrawTelemetryBox(app);

}
