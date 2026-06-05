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

    float textBoxHeight = box.height - BOX_HEADER_SIZE - (margin * 3.0f);
    float textBoxWidth = (box.width - (margin * 3.0f)) / 2.0f;
    float textBoxY = box.y + BOX_HEADER_SIZE + margin*2.0f;

    Rectangle velocity = {box.x + margin, textBoxY, textBoxWidth, textBoxHeight};
    Rectangle altitude = {box.x + margin*2 + textBoxWidth, textBoxY, textBoxWidth, textBoxHeight};

    Fields::field totalVel;
    totalVel.title = "Total Velocity";
    totalVel.val = roundedStr(state.total_velocity) + " mps";

    Fields::field vertVel;
    vertVel.title = "Vertical Velocity";
    vertVel.val = roundedStr(state.sample_buffer.avg_vert_vel_mps(1'000'000), 0) + " mps";

    Fields velocities = {
        {totalVel, vertVel}
    };

    Fields::field altASL{};
    altASL.title = "Alt ASL";
    altASL.val = roundedStr(state.ASL_altitude, 0) + " m";
    
    Fields::field altAGL{};
    altAGL.title = "Alt AGL";
    altAGL.val = roundedStr(state.AGL_altitude, 0) + " m";

    Fields alts = {
        {altAGL, altASL}
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
    // string altAGL = "Altitude AGL: " + roundedStr(state.AGL_altitude);
    // WrittenText altText = {"ALTITUDE", {altASL, altAGL}};

    DrawFieldsInBox(font, READOUT_TEXT_SIZE, velocity, velocities, colors);
    // DrawFieldsInBox(font, BOX_HEADER_SIZE, attitude, attText, colors);
    DrawFieldsInBox(font, READOUT_TEXT_SIZE, altitude, alts, colors);
}
