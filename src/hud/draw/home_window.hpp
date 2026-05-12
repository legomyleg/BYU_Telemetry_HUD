#include "hud/draw/utils.hpp"
#include "hud/hud_app.hpp"
#include "raylib.h"

inline Rectangle DrawCalButton(const HudApp &app) {
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float buttonX = (app.layout.screenWidth + buttonWidth) / 2.0f;
    float buttonY = (app.layout.screenHeight + buttonHeight) / 2.0f;

    Rectangle button = {buttonWidth, buttonHeight, buttonX, buttonY};

    DrawRectangle(0, 0, app.layout.screenWidth, app.layout.screenHeight, app.colors.panelBackgroundColor);
    DrawRectangleRec(button, app.colors.panelColor);
    DrawRectangleRec(button, app.colors.screenDividerColor);
    DrawTextCenteredAtY(app.hudFont, "START CALIBRATING", button, 20.0f, app.colors.textColor, app.layout.screenHeight / 2.0f);

    return button;
}
