#include "hud_app.hpp"

void HudApp::unload() {
    UnloadFont(hudFont);
    UnloadModel(rocket);
    UnloadShader(shader);
    UnloadRenderTexture(sceneTarget);
}

HudApp SetupHudApp() {
    HudApp app{};

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "BYU Telemetry HUD");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    SetTargetFPS(TARGET_FPS);

    app.hudFont = LoadFontEx("resources/fonts/HelveticaNeueRoman.otf", 32, nullptr, 0);
    SetTextureFilter(app.hudFont.texture, TEXTURE_FILTER_BILINEAR);

    app.rocket = LoadModel(MODEL_PATH);

    app.shader = LoadShader(
    "resources/shaders/glsl330/lighting.vs",
    "resources/shaders/glsl330/lighting.fs"
    );

    app.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(app.shader, "viewPos");

    for (int i = 0; i < app.rocket.materialCount; i++) {
        app.rocket.materials[i].shader = app.shader;
    }

    int ambientLoc = GetShaderLocation(app.shader, "ambient");

    float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    SetShaderValue(app.shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    app.light = CreateLight(LIGHT_POINT, {5.0f, 10.0f, 5.0f}, Vector3Zero(), WHITE, app.shader);

    app.camera = {0};
    app.camera.position = {0.0f, 5.0f, 10.0f};
    app.camera.target = {0.0f, 0.0f, 0.0f};
    app.camera.up         = {0.0f, 1.0f, 0.0f};
    app.camera.fovy       = 45.0f;
    app.camera.projection = CAMERA_PERSPECTIVE;

    int sceneWidth = screenWidth * SCENE_WIDTH_RATIO;
    int sceneHeight = screenHeight / 2;
    app.sceneTarget = LoadRenderTexture(sceneWidth * SCENE_RENDER_SCALE,
        sceneHeight * SCENE_RENDER_SCALE);
    SetTextureFilter(app.sceneTarget.texture, TEXTURE_FILTER_BILINEAR);

    app.layout = MakeHudLayout(screenWidth, screenHeight);

    app.state.orientation = QuaternionIdentity();
    app.lastMeasuredTime = 0;

    return app;
}

void UpdateShader(Camera3D camera, Shader shader, Light light) {
    float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
    SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
    UpdateLightValues(shader, light);
}
