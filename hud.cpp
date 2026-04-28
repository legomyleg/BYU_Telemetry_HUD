#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "tools.hpp"
#include "colors.hpp"

constexpr int SAMPLE_RATE = 30;
constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 700;
constexpr int SCENE_WIDTH = SCREEN_WIDTH / 3;
constexpr int SCENE_RENDER_SCALE = 2;
constexpr int PANEL_WIDTH = SCREEN_WIDTH - SCENE_WIDTH;
constexpr int TOP_PANEL_HEIGHT = (SCREEN_HEIGHT - 40) / 3;
constexpr int MIDDLE_PANEL_HEIGHT = TOP_PANEL_HEIGHT;
constexpr int BOTTOM_PANEL_HEIGHT = TOP_PANEL_HEIGHT;

const auto *colors = &MISSION_CONTROL_PALETTE;
float boxRoundness = 0.08;

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rocket Attitude Visualizer");
    SetTargetFPS(SAMPLE_RATE);

    Rectangle sceneRect = {0, 0, (float)SCENE_WIDTH, (float)SCREEN_HEIGHT};
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
        SCREEN_HEIGHT * SCENE_RENDER_SCALE);
    SetTextureFilter(sceneTarget.texture, TEXTURE_FILTER_BILINEAR);

    DataRows running_data = get_data("data/log_0007.csv");

    IMU_Data data = running_data.consume_row();
    auto running_time = data.t_us;
    long long dt;
    GyroSample gyro_vals = {0, 0.0f, 0.0f, 0.0f};

    Quaternion q = QuaternionIdentity();
    while (!WindowShouldClose()) {

        if (!running_data.empty()) {
            data = running_data.consume_row();
            dt = data.t_us - running_time;
            running_time = data.t_us;

            gyro_vals = {dt, data.gx, data.gy, data.gz};

            update_orientation(gyro_vals, q);
            rocket.transform = QuaternionToMatrix(q);
        }

        float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(colors->sceneBackgroundColor); // dark background makes lighting pop

        BeginTextureMode(sceneTarget);
        ClearBackground(colors->sceneBackgroundColor);

        BeginMode3D(camera);
        DrawModel(rocket, {0, 0, 0}, 0.3f, WHITE);
        EndMode3D();

        EndTextureMode();

        DrawTexturePro(
            sceneTarget.texture,
            {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height},
            sceneRect,
            {0, 0},
            0.0f,
            WHITE
        );

        // DrawRectangleRec(panelRect, panelBackgroundColor);
        // DrawLine(SCENE_WIDTH, 0, SCENE_WIDTH, SCREEN_HEIGHT, screenDivideColor);
        // DrawRectangleRounded(topPanel, boxRoundness, 8, panelColor);
        // DrawRectangleRounded(bottomPanel, boxRoundness, 8, panelColor);
        // DrawRectangleRounded(middlePanel, boxRoundness, 8, panelColor);

        DrawRectangleRec(panelRect, colors->panelBackgroundColor);
        DrawLine(SCENE_WIDTH, 0, SCENE_WIDTH, SCREEN_HEIGHT, colors->screenDividerColor);

        DrawRectangleRounded(topPanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(topPanel, 0.08f, 8, colors->panelBorderColor);
        DrawText("ATTITUDE", SCENE_WIDTH + 25, 25, 20, WHITE);
        DrawText(TextFormat("GX: %.3f", data.gx), SCENE_WIDTH + 25, 65, 18, GREEN);
        DrawText(TextFormat("GY: %.3f", data.gy), SCENE_WIDTH + 25, 95, 18, GREEN);
        DrawText(TextFormat("GZ: %.3f", data.gz), SCENE_WIDTH + 25, 125, 18, GREEN);

        DrawRectangleRounded(middlePanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(middlePanel, 0.08f, 8, colors->panelBorderColor);
        DrawText("ALTITUDE / BARO", SCENE_WIDTH + 25, 220, 20, WHITE);
        DrawText(TextFormat("Alt: %.2f m", data.altM), SCENE_WIDTH + 25, 260, 18, LIGHTGRAY);
        DrawText(TextFormat("Press: %.2f Pa", data.pressPa), SCENE_WIDTH + 25, 290, 18, LIGHTGRAY);
        DrawText(TextFormat("Temp: %.2f C", data.baroTempC), SCENE_WIDTH + 25, 320, 18, LIGHTGRAY);

        DrawRectangleRounded(bottomPanel, 0.08f, 8, colors->panelColor);
        DrawRectangleRoundedLines(bottomPanel, 0.08f, 8, colors->panelBorderColor);
        DrawText("FLIGHT STATE", SCENE_WIDTH + 25, 415, 20, WHITE);
        DrawText(TextFormat("Time: %.2f s", data.t_us / 1000000.0f), SCENE_WIDTH + 25, 455, 18, LIGHTGRAY);
        DrawText("Stage: DATA PLAYBACK", SCENE_WIDTH + 25, 485, 18, LIGHTGRAY);
        DrawText(TextFormat("Sample dt: %lld us", dt), SCENE_WIDTH + 25, 515, 18, LIGHTGRAY);


        EndDrawing();
    }
}