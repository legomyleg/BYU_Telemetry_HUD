#include "raylib.h"
#include "../hud_app.hpp"
#include "../colors.hpp"
#include "utils.hpp"

inline void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state) {
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
