#pragma once
#include <string>
#include "colors.hpp"

using std::string;

inline const char* MODEL_PATH = "resources/models/rocket.glb";
inline constexpr int TARGET_FPS = 60;
inline constexpr float SCENE_WIDTH_RATIO = 0.5f;
inline constexpr int SCENE_RENDER_SCALE = 2;
inline constexpr float TEXT_SPACING = 2.0f;
inline constexpr float BOX_ROUNDNESS = 0.08f;
inline constexpr float TEXT_SIZE = 10.0f;
inline constexpr float HEADER_SIZE = 15.0f;
inline constexpr ColorPalette COLOR_PALETTE = WARMER_SPACE_PALETTE_1;

inline constexpr float STAGES_HEIGHT_RATIO = 0.25f;
inline constexpr float SENSORS_HEIGHT_RATIO = 0.5f - STAGES_HEIGHT_RATIO;
inline constexpr float BOTTOM_PANELS_HEIGHT_RATIO = 1.0f - (SENSORS_HEIGHT_RATIO + STAGES_HEIGHT_RATIO);
inline constexpr float GRAPH_WIDTH_RATIO = 0.5f;

inline constexpr int GRAPH_ALT_HEIGHT_M = 10000;
inline constexpr int ALT_BAR_HEIGHT_FT = 30000;
inline const string ALT_BAR_TOP_HEIGHT_TEXT = "30,000 ft";

inline constexpr float M2FT = 3.28084;
inline constexpr float FT2M = 0.3048;