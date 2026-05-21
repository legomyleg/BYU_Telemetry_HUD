#pragma once
#include "raylib.h"

struct ColorPalette {
    const Color sceneBackgroundColor;
    const Color panelBackgroundColor;
    const Color panelColor;
    const Color panelBorderColor;
    const Color screenDividerColor;
    const Color headerTextColor;
    const Color textColor;

    constexpr ColorPalette(Color sbc, Color pbc, Color pc, Color pbdc, Color sdc) : 
    sceneBackgroundColor(sbc),
    panelBackgroundColor(pbc),
    panelColor(pc),
    panelBorderColor(pbdc),
    screenDividerColor(sdc),
    headerTextColor(WHITE),
    textColor(WHITE) {}

    constexpr ColorPalette(Color sbc, Color pbc, Color pc, Color pbdc, Color sdc, Color htc, Color tc) : 
    sceneBackgroundColor(sbc),
    panelBackgroundColor(pbc),
    panelColor(pc),
    panelBorderColor(pbdc),
    screenDividerColor(sdc),
    headerTextColor(htc),
    textColor(tc) {}
};

inline constexpr ColorPalette SPACEY_PALETTE = {
    {10, 14, 24, 255}, // deep space navy
    {22, 31, 45, 255}, // dark blue gray
    {31, 45, 65, 255}, //slightly lighter panel color
    {31, 45, 65, 255},
    {68, 199, 255, 255} // clean cyan accent
};

inline constexpr ColorPalette WARMER_SPACE_PALETTE = {
    {18, 26, 38, 255}, // charcoal space black
    {28, 36, 48, 255}, // warm slate background
    {42, 52, 66, 255}, // soft steel blue panel
    {42, 52, 66, 255}, 
    {255, 183, 77, 255} // amber mission accent
};

inline constexpr ColorPalette WARMER_SPACE_PALETTE_1 = {
    {18, 26, 38, 255}, // charcoal space black
    {28, 36, 48, 255}, // warm slate background
    {42, 52, 66, 255}, // soft steel blue panel
    {42, 52, 66, 255}, 
    {255, 183, 77, 255}, // amber mission accent
    WHITE,
    GREEN
};

inline constexpr ColorPalette CYBER_HUD = {
    {8, 12, 20, 255}, // cyber midnight
    {13, 24, 36, 255}, // deep teal navy
    {18, 38, 52, 255}, // muted cyber teal
    {18, 38, 52, 255}, 
    {0, 255, 200, 255} // neon cyan mint
};

inline constexpr ColorPalette MISSION_CONTROL_PALETTE = {

    {12, 14, 22, 255},    // near-black navy panel background
    {12, 14, 22, 255},
    {25, 30, 45, 255},    // dark aerospace blue panel
    {90, 105, 130, 255},  // soft steel outline
    {80, 90, 110, 255} // muted slate divider
};

inline constexpr ColorPalette MISSION_CONTROL_PALETTE_1 = {

    {12, 14, 22, 255},    // near-black navy panel background
    {12, 14, 22, 255},
    {25, 30, 45, 255},    // dark aerospace blue panel
    {90, 105, 130, 255},  // soft steel outline
    {80, 90, 110, 255}, // muted slate divider
    WHITE,
    GREEN
};