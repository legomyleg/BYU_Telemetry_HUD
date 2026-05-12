#include "hud/draw/utils.hpp"
#include "hud/hud_app.hpp"
#include "raylib.h"

inline Rectangle DrawCalibScreen(const HudApp &app) {
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float buttonX = (app.layout.screenWidth - buttonWidth) / 2.0f;
    float buttonY = (app.layout.screenHeight - buttonHeight) / 2.0f;

    Rectangle button = {buttonX, buttonY, buttonWidth, buttonHeight};

    DrawRectangle(0, 0, app.layout.screenWidth, app.layout.screenHeight, app.colors.panelBackgroundColor);
    DrawRectangleRec(button, app.colors.panelColor);
    DrawRectangleLinesEx(button, 2.0f, app.colors.screenDividerColor);
    DrawTextCenteredEx(app.hudFont, "CALIBRATE", button, 20.0f, app.colors.headerTextColor);

    return button;
}
