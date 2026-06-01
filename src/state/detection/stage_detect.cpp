#include "state/rocket_state.hpp"
#include "telemetry/sample_ring_buffer.hpp"
#include "telemetry/telemetry_config.hpp"
#include <cmath>
#include <state/detection/stage_detect.hpp>

float get_mag(Vector3 v) {
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

void handle_calib(RocketState& state) {
    if (state.sample_buffer.duration_full()) {
        return;
    }

    state.biases.accel = state.sample_buffer.avg_accel_all();
    state.biases.gyro = state.sample_buffer.avg_gyro_all();

    state.transition_to(FlightStage::Pad);
}

void handle_pad(RocketState& state) {
    auto accel = state.sample_buffer.avg_accel(HALF_SECOND);
    float mag = std::sqrtf(accel.x*accel.x + accel.y*accel.y + accel.z*accel.z);
    if (mag > 50.0f) {
        state.transition_to(FlightStage::Boost);
    }
}

void handle_boost(RocketState& state) {
    constexpr uint64_t ONE_THIRD_SEC = 300'000;
    auto mag = get_mag(state.sample_buffer.avg_accel(ONE_THIRD_SEC));
    if (mag < 15.0f && state.time_in_stage() > ONE_SECOND) {
        state.transition_to(FlightStage::Coast);
    }
}

void handle_coast(RocketState& state) {
    auto vert_vel = state.sample_buffer.avg_vert_vel_mps(HALF_SECOND);
    if (state.time_in_stage() > 10'000'000 && vert_vel < 10.0f) {
        state.transition_to(FlightStage::Apogee);
    }
}

void handle_apogee(RocketState& state) {
    if (state.time_in_stage() > 5'000'000) {
        state.transition_to(FlightStage::Descent);
    }
}

void handle_descent(RocketState& state) {
    auto vert_vel = state.sample_buffer.avg_vert_vel_mps(ONE_SECOND);
    if (vert_vel < 2.0f) {
        state.transition_to(FlightStage::Recovery);
    }
}

void StageDetect::update(RocketState& state) {

    switch (state.stage) {
        case FlightStage::Calibrating: 
            handle_calib(state);
        
        case FlightStage::Pad:
            handle_pad(state);
            break;
        case FlightStage::Boost:
            handle_boost(state);
            break;
        case FlightStage::Coast:
            handle_coast(state);
            break;
        case FlightStage::Apogee:
            handle_apogee(state);
            break;
        case FlightStage::Descent:
            handle_descent(state);
            break;
        case FlightStage::Recovery:
            break;
    }
}
