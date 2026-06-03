#pragma once
#include "state/calibration.hpp"
#include <hud/hud_app.hpp>
#include <raylib.h>
#include <telemetry/sample_buffer.hpp>
#include <telemetry/sensor_data.hpp>
#include <state/rocket_state.hpp>
#include <telemetry/telem_source.hpp>

void ReadSamples(HudApp& app, TelemetrySource &tsrc);

void update_vertical_velocity(float da, float dt_s, float &vert_velocity);

void update_velocity(SensorData &s, float dt_s, RocketState& state, Biases& biases);

void update_orientation(const SensorData sample, const float dt_s, Quaternion& orientation);

void UpdateState(HudApp& app, SampleQueue& samples, TelemetrySource &tsrc);
