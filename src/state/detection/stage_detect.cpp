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
    auto mag = get_mag(state.sample_buffer.avg_accel(ONE_SECOND));
}

void StageDetect::update(RocketState& state) {

    switch (state.stage) {
        case FlightStage::Calibrating: 
            handle_calib(state);
        
        case FlightStage::Pad:
            handle_pad(state);
            
        case FlightStage::Boost:
            // Later
        case FlightStage::Coast:
            // Later
        case FlightStage::Apogee:
            // Later
        case FlightStage::Descent:
            // Later
        case FlightStage::Recovery:
            // Later
    }
}
