cmake_minimum_required(VERSION 3.20)

project(BYU_Telemetry_HUD VERSION 1.0 LANGUAGES CXX)

find_package(raylib CONFIG REQUIRED)

add_executable(BYU_Telemetry_HUD
        src/main.cpp
        src/hud/hud_app.cpp
        src/hud/hud_draw.cpp
        src/state/state_update.cpp
        src/telemetry/serial.cpp
        src/telemetry/telemetry_parse.cpp
)

target_compile_features(BYU_Telemetry_HUD PRIVATE cxx_std_20)

target_include_directories(BYU_Telemetry_HUD PRIVATE
        src
)

target_link_libraries(BYU_Telemetry_HUD PRIVATE
        raylib
)
