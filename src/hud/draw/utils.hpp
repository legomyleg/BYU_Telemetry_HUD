#pragma once

#include "../config.hpp"
#include "raylib.h"
#include "../hud_app.hpp"
#include "../colors.hpp"
#include <format>
#include <string>
using std::format, std::string;

struct WrittenText {
    string title;
    vector<string> lines;
};

inline string roundedStr(float val) {
    return format("{:.2f}", val);
}

inline void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, const Color color) {
    Vector2 size = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

inline void DrawTextCenteredToTop(Font font, const char *text, Rectangle rect, float fontSize, const Color color, float margin) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
        rect.x + (rect.width - textSize.x) / 2,
        rect.y + margin
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

inline void DrawTextCenteredAtY(Font font, const char* text, Rectangle rect, float fontSize, const Color color, float y) {
    // Y val is absolute, relative to the screen, NOT the rect

    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
            rect.x + (rect.width - textSize.x) / 2,
            y
    };

    DrawTextEx(font, text, position, fontSize, TEXT_SPACING, color);
}

inline void DrawTextCenteredAtXVertical(Font font, const char* text, Rectangle rect, float fontSize, const Color color, float x) {
    // X val is relative to screen, NOT rect
    Vector2 textSize = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
            x,
            rect.y + (rect.height + textSize.x) / 2
    };

    DrawTextPro(font, text, position, Vector2{0,0}, -90.0f, fontSize, TEXT_SPACING, color);
}

inline void DrawFieldsInBox(Font font, Rectangle& box, WrittenText &text, const ColorPalette &colors, float gapSize) {
    DrawTextCenteredToTop(font, text.title.c_str(), box, SUBHEADER_TEXT_SIZE, colors.headerTextColor, 0);

    float centeredX = (box.x + box.width) / 2.0f;
    Vector2 nextPos = {centeredX, box.y + SUBHEADER_TEXT_SIZE + gapSize};
    for (const auto & line : text.lines) {
        DrawTextCenteredAtY(font, line.c_str(), box, READOUT_TEXT_SIZE, colors.textColor, nextPos.y);
        nextPos.y += READOUT_TEXT_SIZE + gapSize;
    }
}
