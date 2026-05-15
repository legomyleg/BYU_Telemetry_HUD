cmake_minimum_required(VERSION 3.20)

project(BYU_Telemetry_HUD VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)

add_executable(BYU_Telemetry_HUD
        src/main.cpp
        src/hud/run_hud.cpp
        src/hud/hud_app.cpp
        src/hud/draw/hud_draw.cpp
        src/state/state_update.cpp
        src/telemetry/serial.cpp
        src/telemetry/telemetry_parse.cpp
        src/hud/draw/telem.cpp
)

add_executable(test 
        tests/test.cpp
        src/hud/run_hud.cpp
        src/hud/draw/hud_draw.cpp
        src/hud/hud_app.cpp
        src/hud/draw/telem.cpp
        src/telemetry/telemetry_parse.cpp
        src/telemetry/CsvTelemSource.cpp
        src/state/state_update.cpp
)

add_executable(raven_test
        tests/raven_test/main.cpp
        src/hud/run_hud.cpp
        src/hud/draw/hud_draw.cpp
        src/hud/hud_app.cpp
        src/hud/draw/telem.cpp
        src/telemetry/telemetry_parse.cpp
        src/telemetry/CsvTelemSource.cpp
        src/state/state_update.cpp
)

target_compile_features(BYU_Telemetry_HUD PRIVATE cxx_std_20)
target_compile_features(test PRIVATE cxx_std_20)
target_compile_features(raven_test PRIVATE cxx_std_20)

target_include_directories(BYU_Telemetry_HUD PRIVATE
        src
)
target_include_directories(test PRIVATE src)
target_include_directories(raven_test PRIVATE src)

target_link_libraries(BYU_Telemetry_HUD PRIVATE
        raylib
)
target_link_libraries(test PRIVATE raylib)
target_link_libraries(raven_test PRIVATE raylib)
