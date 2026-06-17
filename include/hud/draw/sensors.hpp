#pragma once
#include "hud/config.hpp"
#include "hud/hud_layout.hpp"
#include "state/rocket_state.hpp"
#include <cmath>
#include <raylib.h>
#include <hud/hud_app.hpp>
#include <hud/colors.hpp>
#include <hud/draw/utils.hpp>
#include <string>
using std::string;

struct GaugeParams {
    Font font;
    Vector2 center;
    float radius;
    float value;
    float minValue;
    float maxValue;
};

inline void DrawCircleArc(Vector2 center, float radius, float startAngle, float endAngle, int segments, float thickness, Color color) {
    if (segments < 2) segments = 2;

    Vector2 points[segments];
    float angleStep = (endAngle - startAngle) / (float)(segments - 1);

    for (int i=0; i < segments; i++) {
        float currentAngleRad = (startAngle + (angleStep * i)) * DEG2RAD;


        points[i].x = center.x + cosf(currentAngleRad) * radius;
        points[i].y = center.y + sinf(currentAngleRad) * radius;
    }

    for (int i=0; i < segments - 1; i++) {
        DrawLineEx(points[i], points[i + 1], thickness, color);
    }
}

inline Color GetGaugeColor(float currentAngle, float minAngle, float maxAngle) {
    if (currentAngle < minAngle) currentAngle = minAngle;
    if (currentAngle > maxAngle) currentAngle = maxAngle;

    float t = (currentAngle - minAngle) / (maxAngle - minAngle);

    unsigned char r = (unsigned char)(0 + (t * 255));
    unsigned char g = (255 + (0 - 255) * t);
    unsigned char b = 0;
    unsigned char a = 255;

    return (Color){ r, g, b, a };
}

inline void DrawGauge(GaugeParams& params) {
    if (params.value < params.minValue) params.value = params.minValue;
    if (params.value > params.maxValue) params.value = params.maxValue;

    float startAngle = 135.0f;
    float endAngle = 405.0f;
    float t = (params.value - params.minValue) / (params.maxValue - params.minValue);
    float angle = startAngle + t * (endAngle - startAngle);

    DrawCircleArc(params.center, params.radius, startAngle, endAngle, 64, 5, GRAY);
    auto color = GetGaugeColor(angle, startAngle, endAngle);
    // Color color = GREEN;
    DrawCircleArc(params.center, params.radius, startAngle, angle, 64, 5, color);

    int tickCount = 6;
    for (int i = 0; i <= tickCount; i++) {
        float tickT = (float)i / tickCount;
        float tickAngle = startAngle + tickT * (endAngle - startAngle);
        float rad = tickAngle * DEG2RAD;

        Vector2 outer = {
            params.center.x + cosf(rad) * params.radius,
            params.center.y + sinf(rad) * params.radius
        };

        float tickLen = params.radius / 6.0f;
        Vector2 inner = {
            params.center.x + cosf(rad) * (params.radius - tickLen),
            params.center.y + sinf(rad) * (params.radius - tickLen)
        };

        DrawLineEx(inner, outer, 2.0f,  LIGHTGRAY);
    }

    float rad = angle * DEG2RAD;
    Vector2 needleStart = {
        params.center.x + cosf(rad) * (params.radius - 3.0f),
        params.center.y + sinf(rad) * (params.radius - 3.0f)
    };
    Vector2 needleEnd = {
        params.center.x + cosf(rad) * (params.radius + 3.0f),
        params.center.y + sinf(rad) * (params.radius + 3.0f)
    };
    DrawLineEx(needleStart, needleEnd, 2.0f, RED);

    string valueText = TextFormat("%.1f", params.value);
    Vector2 textSize = MeasureTextEx(params.font, valueText.c_str(), 15, TEXT_SPACING);
    Vector2 textPos = {
        params.center.x - (textSize.x / 2),
        params.center.y - (textSize.y / 2)
    };
    DrawTextEx(params.font, valueText.c_str(), textPos, 15, TEXT_SPACING, WHITE);
}

inline void DrawGaugeInBox(GaugeParams& params, Rectangle rect, const char* label, float fontSize) {
    float margin = 15.0f;

    Vector2 gauge_pos = {
        rect.x + rect.width - margin - params.radius,
        rect.y + (rect.height / 2.0f) + 3.0f
    };

    params.center = gauge_pos;

    auto textSize = MeasureTextEx(params.font, label, fontSize, TEXT_SPACING);
    Vector2 textPos = {
        rect.x + margin,
        rect.y + (rect.height / 2.0f) - 5.0f
    };

    DrawTextEx(params.font, label, textPos, fontSize, TEXT_SPACING, WHITE);
    DrawGauge(params);
}

inline void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state) {
    float margin = 15.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "SENSOR DATA", box, BOX_HEADER_SIZE, colors.headerTextColor, margin);

    float textBoxHeight = (box.height - BOX_HEADER_SIZE - (margin * 4.0f)) / 2.0f;
    float textBoxWidth = (box.width - (margin * 3.0f)) / 2.0f;
    float topY = box.y + BOX_HEADER_SIZE + margin*2.0f;
    float botY = topY + textBoxHeight + margin;
    float leftX = box.x + margin;
    float rightX = leftX + textBoxWidth + margin;

    float w = box.width - (2.0f * margin);
    Rectangle GsBox = {leftX, topY, textBoxWidth, textBoxHeight};
    Rectangle tVelBoc  = {rightX, topY, textBoxWidth, textBoxHeight};
    Rectangle velocityBox = {leftX, botY, textBoxWidth, textBoxHeight};
    Rectangle altitudeBox = {rightX, botY, textBoxWidth, textBoxHeight};

    DrawRectangleRoundedLines(GsBox, BOX_ROUNDNESS, 8, colors.screenDividerColor);
    DrawRectangleRoundedLines(tVelBoc, BOX_ROUNDNESS, 8, colors.screenDividerColor);
    Vector2 pos = {GsBox.x + (GsBox.width / 2.0f), GsBox.y + (GsBox.height / 2) + 5};
    auto avg_vel = state.sample_buffer.avg_vert_vel_mps(2'000'000);

    GaugeParams p = {
        font, 
        {},
        40.0f,
        state.g_force,
        0,
        25
    };

    GaugeParams params = {
        font,
        {},
        40.0f,
        avg_vel,
        -900,
        900
    };

    DrawGaugeInBox(p, GsBox, "G Force: ", READOUT_TEXT_SIZE);
    DrawGaugeInBox(params, tVelBoc, "Vertical Velocity: ", READOUT_TEXT_SIZE);
    // DrawGauge(font, pos, 40.0f, avg_vel, 0, 25);

    // Fields::field vertVel;
    // vertVel.title = "Vertical Velocity";
    // vertVel.val = roundedStr(state.sample_buffer.avg_vert_vel_mps(1'000'000), 0) + " m/s";

    Fields::field rollRate;
    rollRate.title = "Roll Rate";
    rollRate.val = roundedStr(state.attitude_roc.z) + " rad/s";

    Fields::field pitchRate;
    pitchRate.title = "Pitch Rate";
    pitchRate.val = roundedStr(state.attitude_roc.x) + " rad/s";

    Fields::field yawRate;
    yawRate.title = "Yaw Rate";
    yawRate.val = roundedStr(state.attitude_roc.y) + " rad/s";

    Fields velocities = {
        {rollRate, pitchRate, yawRate}
    };

    Fields::field altASL{};
    altASL.title = "Alt ASL";
    altASL.val = roundedStr(state.ASL_altitude * M2FT, 0) + " ft";
    
    Fields::field altAGL{};
    altAGL.title = "Alt AGL";
    altAGL.val = roundedStr(state.AGL_altitude * M2FT, 0) + " ft";

    Fields alts = {
        {altAGL, altASL}
    };

    Fields::field samplesPerSec;
    samplesPerSec.title = "Received Samples /s";
    samplesPerSec.val = roundedStr(state.samples_per_sec, 1);

    Fields sps = {
        {samplesPerSec}
    };

    Fields::field packets_dropped;
    packets_dropped.title = "Packets Dropped";
    packets_dropped.val = "";

    Fields pd = {
        {packets_dropped}
    };

    DrawFieldsInBox(font, READOUT_TEXT_SIZE, velocityBox, velocities, colors);
    DrawFieldsInBox(font, READOUT_TEXT_SIZE, altitudeBox, alts, colors);
    // DrawFieldsInBox(font, READOUT_TEXT_SIZE, samplesPerSecBox, sps, colors);
    // DrawFieldsInBox(font, READOUT_TEXT_SIZE, packetsDroppedBox, pd, colors);
}
