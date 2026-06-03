#include "hud/config.hpp"
#include <raylib.h>
#include <hud/hud_app.hpp>
#include <hud/colors.hpp>
#include <hud/draw/utils.hpp>

inline void DrawSensorsBox(Font font, const HudBox &box, const ColorPalette &colors, const RocketState& state) {
    float margin = 15.0f;

    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.panelBorderColor);
    DrawTextCenteredToTop(font, "SENSOR DATA", box, BOX_HEADER_SIZE, colors.headerTextColor, margin);

    float textBoxHeight = box.height - BOX_HEADER_SIZE - (margin * 3);
    float textBoxWidth = (box.width - (margin * 4)) / 3.0f;
    float textBoxY = box.y + BOX_HEADER_SIZE + margin*2;

    Rectangle velocity = {box.x + margin, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle attitude = {box.x + (margin*2) + textBoxWidth, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle altitude = {box.x + margin*3 + textBoxWidth*2, textBoxY, textBoxWidth, textBoxHeight};

    WrittenText::input totalVel;
    totalVel.title = "Total Velocity";
    totalVel.val = roundedStr(state.total_velocity);

    WrittenText::input vertVel;
    vertVel.title = "Vertical Velocity";
    vertVel.val = roundedStr(state.vertical_velocity_mps, 0);

    WrittenText velocities = {
        {totalVel, vertVel}
    };

            // string velXyz = "X: " + roundedStr(state.velocity.x) +
            //         " Y: " + roundedStr(state.velocity.y) +
            //         " Z: " + roundedStr(state.velocity.z);
            // string totalVel = "Total Velocity: " + roundedStr(state.total_velocity) + "m/s";
            // string vertVel = "Vertical Velocity: " + roundedStr(state.vertical_velocity_mps) + "m/s";
            // vector<string> vDataVals = {velXyz, totalVel, vertVel};
            // WrittenText velText = {"VELOCITY", vDataVals};

    // string attRoll = "Roll: " + roundedStr(state.attitude.roll);
    // string attPitch = "Pitch: " + roundedStr(state.attitude.pitch);
    // string attYaw = "Yaw: " + roundedStr(state.attitude.yaw);
    // vector<string> attDataVals = {attRoll, attPitch, attYaw};
    // WrittenText attText = {"ATTITUDE", attDataVals};
    //
    // string altASL = "Altitude ASL: " + roundedStr(state.altitude);
    // string altAGL = "Altitude AGL: " + roundedStr(state.altAGL());
    // WrittenText altText = {"ALTITUDE", {altASL, altAGL}};

    DrawFieldsInBox(font, BOX_HEADER_SIZE, velocity, velocities, colors);
    // DrawFieldsInBox(font, BOX_HEADER_SIZE, attitude, attText, colors);
    // DrawFieldsInBox(font, BOX_HEADER_SIZE, altitude, altText, colors);
}
