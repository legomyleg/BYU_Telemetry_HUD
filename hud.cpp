#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "tools.hpp"

constexpr int SAMPLE_RATE = 30;
constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;
constexpr int SCENE_WIDTH = SCREEN_WIDTH / 3;
constexpr int PANEL_WIDTH = SCREEN_WIDTH - SCENE_WIDTH;
constexpr int TOP_PANEL_HEIGHT = (SCREEN_HEIGHT / 3) - 20;
constexpr int BOTTOM_PANEL_HEIGHT = SCREEN_HEIGHT - (TOP_PANEL_HEIGHT + 20) - 20;

Color renderingBackground = {20, 20, 30, 255};
Color panelBackgroundColor = {80, 90, 110, 255};
Color panelColor = {110, 55, 61, 255};
Color screenDivideColor = {106, 247, 245, 255};
float boxRoundness = 0.08;

int main() {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rocket Attitude Visualizer");
    SetTargetFPS(SAMPLE_RATE);

    Rectangle sceneRect = {0, 0, (float)SCENE_WIDTH, (float)SCREEN_HEIGHT};
    Rectangle panelRect = {(float)SCENE_WIDTH, 0, (float)PANEL_WIDTH, (float)SCREEN_HEIGHT};
    Rectangle topPanel = {(float)SCENE_WIDTH + 10, 10, (float)PANEL_WIDTH - 20, (float)TOP_PANEL_HEIGHT};
    Rectangle bottomPanel = {(float)SCENE_WIDTH + 10, (float)TOP_PANEL_HEIGHT + 20, (float)PANEL_WIDTH, (float)BOTTOM_PANEL_HEIGHT};

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

    RenderTexture2D sceneTarget = LoadRenderTexture(SCENE_WIDTH, SCREEN_HEIGHT);

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
        ClearBackground(renderingBackground); // dark background makes lighting pop

        BeginTextureMode(sceneTarget);
        ClearBackground(renderingBackground);

        BeginMode3D(camera);
        DrawModel(rocket, {0, 0, 0}, 0.3f, WHITE);
        DrawGrid(10, 1.0f);
        EndMode3D();

        EndTextureMode();

        DrawTextureRec(
            sceneTarget.texture,
            {0, 0, (float)sceneTarget.texture.width, -(float)sceneTarget.texture.height},
            {0, 0},
            WHITE
        );

        DrawRectangleRec(panelRect, panelBackgroundColor);
        DrawLine(SCENE_WIDTH, 0, SCENE_WIDTH, SCREEN_HEIGHT, screenDivideColor);
        DrawRectangleRounded(topPanel, boxRoundness, 8, panelColor);
        EndDrawing();
    }
}