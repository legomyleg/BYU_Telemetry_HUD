#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "tools.hpp"
#include "colors.hpp"
#include "hud_draw.hpp"

constexpr int SAMPLE_RATE = 30;
constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 700;
constexpr int SCENE_WIDTH = SCREEN_WIDTH / 3;
constexpr int SCENE_HEIGHT = SCREEN_HEIGHT / 2;
constexpr int SCENE_RENDER_SCALE = 2;
constexpr int PANEL_WIDTH = SCREEN_WIDTH - SCENE_WIDTH;
constexpr int TOP_PANEL_HEIGHT = (SCREEN_HEIGHT - 40) / 5;
constexpr int MIDDLE_PANEL_HEIGHT = TOP_PANEL_HEIGHT;
constexpr int BOTTOM_PANEL_HEIGHT = TOP_PANEL_HEIGHT;

const auto *colors = &WARMER_SPACE_PALETTE_1;
float boxRoundness = 0.08;
const float TEXT_SPACING = 2.0f;


int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rocket Attitude Visualizer");
    SetWindowMinSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(SAMPLE_RATE);
    
    Font hudFont = LoadFontEx("resources/fonts/HelveticaNeueRoman.otf", 32, nullptr, 0);
    SetTextureFilter(hudFont.texture, TEXTURE_FILTER_BILINEAR);

    Rectangle sceneRect = {0, 0, (float)SCENE_WIDTH, (float)SCENE_HEIGHT};
    Rectangle cameraFeedRect = {0, (float)SCENE_HEIGHT, (float)SCENE_WIDTH, (float)SCENE_HEIGHT};
    Rectangle panelRect = {(float)SCENE_WIDTH, 0, (float)PANEL_WIDTH, (float)SCREEN_HEIGHT};
    Rectangle topPanel = {(float)SCENE_WIDTH + 10, 10, (float)PANEL_WIDTH - 20, (float)TOP_PANEL_HEIGHT};
    Rectangle middlePanel = {(float)SCENE_WIDTH + 10, (float)TOP_PANEL_HEIGHT + 20, (float)PANEL_WIDTH - 20, (float)MIDDLE_PANEL_HEIGHT};
    Rectangle bottomPanel = {(float)SCENE_WIDTH + 10, SCREEN_HEIGHT - (BOTTOM_PANEL_HEIGHT + 10), (float)PANEL_WIDTH - 20, (float)BOTTOM_PANEL_HEIGHT};

    Model rocket = LoadModel("resources/models/rocket.glb");

    Shader shader = LoadShader(
    "resources/shaders/glsl330/lighting.vs",
    "resources/shaders/glsl330/lighting.fs"
    );

    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    for (int i = 0; i < rocket.materialCount; i++) {
        rocket.materials[i].shader = shader;
    }

    int ambientLoc = GetShaderLocation(shader, "ambient");

    float ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);

    Light light = CreateLight(LIGHT_POINT, {5.0f, 10.0f, 5.0f}, Vector3Zero(), WHITE, shader);

    Camera3D camera = {0};
    camera.position = {0.0f, 5.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up         = {0.0f, 1.0f, 0.0f};
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    RenderTexture2D sceneTarget = LoadRenderTexture(SCENE_WIDTH * SCENE_RENDER_SCALE, 
        SCENE_HEIGHT * SCENE_RENDER_SCALE);
    SetTextureFilter(sceneTarget.texture, TEXTURE_FILTER_BILINEAR);

    DataRows running_data = get_data("data/log_0007.csv");

    IMU_Data data = running_data.consume_row();
    auto running_time = data.t_us;
    long long dt;
    GyroSample gyro_vals = {0.0f, 0.0f, 0.0f};

    Quaternion q = QuaternionIdentity();

    HudLayout layout = MakeHudLayout(SCREEN_WIDTH, SCREEN_HEIGHT);
    while (!WindowShouldClose()) {

        if (!running_data.empty()) {
            data = running_data.consume_row();
            dt = data.t_us - running_time;
            running_time = data.t_us;

            gyro_vals = {data.gx, data.gy, data.gz};

            update_orientation(gyro_vals, dt, q);
            rocket.transform = QuaternionToMatrix(q);
        }

        float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);

        // --- DRAW ---
        DrawSceneBox(layout.scene, sceneTarget, camera, rocket, colors->sceneBackgroundColor);
        DrawCameraFeedBox(hudFont, layout.cameraFeed, colors->screenDividerColor);

        DrawRectangleRec(panelRect, colors->panelBackgroundColor);
        DrawLine(SCENE_WIDTH, 0, SCENE_WIDTH, SCREEN_HEIGHT, colors->screenDividerColor);

        

        DrawRectangleRounded(topPanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(topPanel, 0.08f, 8, colors->panelBorderColor);
        DrawTextEx(hudFont, "ATTITUDE", {(float)SCENE_WIDTH + 25, topPanel.y + 10}, 20, TEXT_SPACING, colors->headerTextColor);
        DrawTextEx(hudFont, TextFormat("GX: %.3f", data.gx), {(float)SCENE_WIDTH + 25, topPanel.y + 55}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, TextFormat("GY: %.3f", data.gy), {(float)SCENE_WIDTH + 25, topPanel.y + 85}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, TextFormat("GZ: %.3f", data.gz), {(float)SCENE_WIDTH + 25, topPanel.y + 115}, 18, TEXT_SPACING, colors->textColor);

        DrawRectangleRounded(middlePanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(middlePanel, 0.08f, 8, colors->panelBorderColor);
        DrawTextEx(hudFont, "ALTITUDE / BARO", {(float)SCENE_WIDTH + 25, middlePanel.y + 10}, 20, TEXT_SPACING, colors->headerTextColor);
        DrawTextEx(hudFont, TextFormat("Alt: %.2f m", data.altM), {(float)SCENE_WIDTH + 25, middlePanel.y + 55}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, TextFormat("Press: %.2f Pa", data.pressPa), {(float)SCENE_WIDTH + 25, middlePanel.y + 85}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, TextFormat("Temp: %.2f C", data.baroTempC), {(float)SCENE_WIDTH + 25, middlePanel.y + 115}, 18, TEXT_SPACING, colors->textColor);

        DrawRectangleRounded(bottomPanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(bottomPanel, 0.08f, 8, colors->panelBorderColor);
        DrawTextEx(hudFont, "FLIGHT STATE", {(float)SCENE_WIDTH + 25, bottomPanel.y + 10}, 20, TEXT_SPACING, colors->headerTextColor);
        DrawTextEx(hudFont, TextFormat("Time: %.2f s", data.t_us / 1000000.0f), {(float)SCENE_WIDTH + 25, bottomPanel.y + 55}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, "Stage: DATA PLAYBACK", {(float)SCENE_WIDTH + 25, bottomPanel.y + 85}, 18, TEXT_SPACING, colors->textColor);
        DrawTextEx(hudFont, TextFormat("Sample dt: %lld us", dt), {(float)SCENE_WIDTH + 25, bottomPanel.y + 115}, 18, TEXT_SPACING, colors->textColor);


        EndDrawing();
    }
    UnloadFont(hudFont);
}
