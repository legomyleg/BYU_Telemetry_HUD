#pragma once
#include "hud/hud_app.hpp"
#include "telemetry/sample_buffer.hpp"
#include "telemetry/sensor_data.hpp"
#include "rocket_state.hpp"
#include "telemetry/telem_source.hpp"

void ReadSamples(HudApp& app, TelemetrySource &tsrc);

void update_vertical_velocity(float da, float dt_s, float &vert_velocity);

void update_velocity(SensorData &s, float dt_s, Vec3 &velocity);

void update_orientation(const SensorData sample, const float dt_s, Quaternion& orientation);

void UpdateState(HudApp& app, SampleBuffer& samples, TelemetrySource &tsrc);
