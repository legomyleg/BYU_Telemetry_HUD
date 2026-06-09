cmake_minimum_required(VERSION 3.14)
project(HUDTestSuite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(raylib REQUIRED)

add_executable(run_tests
    ./unit_tests/state_update_tests.cpp
    ./unit_tests/telemetry_parse_tests.cpp
    ../src/hud/hud_app.cpp
    ../src/state/state_update.cpp
    ../src/telemetry/telemetry_parse.cpp
    ../src/state/detection/stage_detect.cpp
)

target_include_directories(run_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ../include
)

target_link_libraries(run_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    Threads::Threads
    raylib
)

if (APPLE)
    target_compile_options(run_tests PRIVATE -isystem /opt/homebrew/include/)
endif()
