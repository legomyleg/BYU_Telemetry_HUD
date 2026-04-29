#include "hud_draw.hpp"
constexpr float TEXT_SPACING = 2.0f;

void DrawTextCenteredEx(Font font, const char *text, Rectangle rect, float fontSize, float spacing, Color color) {
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = {
        rect.x + (rect.width - size.x) / 2,
        rect.y + (rect.height - size.y) / 2
    };

    DrawTextEx(font, text, position, fontSize, spacing, color);
}

void DrawSceneBox(const HudBox &box, 
    RenderTexture2D &sceneTarget, 
    const Camera3D &camera, 
    const Model &rocket,
    const Color backgroundColor
) {
    BeginTextureMode(sceneTarget);
    ClearBackground(backgroundColor);

    BeginMode3D(camera);
    DrawModel(rocket, {0, 0, 0}, 0.3f, WHITE);
    EndMode3D();

    EndTextureMode();

    DrawTexturePro(
        sceneTarget.texture,
        {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height},
        box.bounds,
        {0, 0},
        0.0f,
        WHITE
    );
}

void DrawCameraFeedBox(Font font, const HudBox &box, const Color dividerColor) {
    DrawRectangleRec(box.bounds, {0,0,0,255});
    DrawLine(0, box.bounds.height, box.bounds.width, box.bounds.height, dividerColor);
    DrawTextCenteredEx(font, "CAMERA FEED", box.bounds, 35, TEXT_SPACING, WHITE);
}
