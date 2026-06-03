#pragma once

#include <hud/config.hpp>
#include "raylib.h"
#include <hud/hud_app.hpp>
#include <hud/colors.hpp>
#include <format>
#include <string>
using std::format, std::string;

inline constexpr float OPTICAL_OFFSET = 2.0f;

struct Fields {
    struct field {
        string title;
        string val;
    };

    vector<field> fields;
};

inline string roundedStr(float val, int precision=2) {
    if (precision == 0) return format("{}", static_cast<int>(val));
    return format("{:.{}f}", val, precision);
}

inline void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, const Color color) {
    Vector2 size = MeasureTextEx(font, text, fontSize, TEXT_SPACING);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2 + OPTICAL_OFFSET
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
            y - (fontSize / 2.0f)
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

inline void DrawFieldsInBox(Font font, float fontSize, Rectangle& box, Fields &text, const ColorPalette &colors) {
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.screenDividerColor);

    const int num_gaps = text.fields.size() + 1;
    const float total_gap_len = box.height - (text.fields.size() * fontSize);
    const float gap_size = total_gap_len / (float)num_gaps;

    float margin_to_side = 10.0f;
    
    float x_pos_title = box.x + margin_to_side;
    auto calc_x_pos_side = [=](string text) {
        auto size = MeasureTextEx(font, text.c_str(), fontSize, TEXT_SPACING);
        return (box.x + box.width - size.x - margin_to_side);
    };
    float y_pos = box.y + gap_size;

    for (const auto &line : text.fields) {
        Vector2 pos_title = {x_pos_title, y_pos};
        DrawTextEx(font, line.title.c_str(), pos_title, fontSize, TEXT_SPACING, colors.headerTextColor);

        Vector2 pos_val = {calc_x_pos_side(line.val), y_pos};
        DrawTextEx(font, line.val.c_str(), pos_val, fontSize, TEXT_SPACING, colors.textColor);

        y_pos += gap_size + fontSize;
    }
}

inline void DrawFieldsWithSubheaders(Font font, Rectangle& box, Fields::field text, const ColorPalette &colors, float sh_size, float ro_size, float gapSize) {
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, colors.screenDividerColor);
    DrawTextCenteredToTop(font, text.title.c_str(), box, sh_size, colors.headerTextColor, FB_TITLE_MARGIN_TOP);

    float centeredX = (box.x + box.width) / 2.0f;
    Vector2 nextPos = {centeredX, box.y + sh_size + (2.0f*FB_TITLE_MARGIN_BOT)};
    DrawTextCenteredAtY(font, text.val.c_str(), box, ro_size, colors.textColor, nextPos.y);

    // for (const auto & line : text.fields) {
    //     DrawTextCenteredAtY(font, line.c_str(), box, ro_size, colors.textColor, nextPos.y);
    //     nextPos.y += ro_size + gapSize;
    // }
}
