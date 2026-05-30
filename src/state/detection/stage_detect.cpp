#include "state/rocket_state.hpp"
#include "telemetry/sample_ring_buffer.hpp"
#include <state/detection/stage_detect.hpp>

void handle_calib(RocketState& state) {
    if (state.sample_buffer.duration_full()) {
        return;
    }

    state.biases.accel = state.sample_buffer.avg_accel_all();
    state.biases.gyro = state.sample_buffer.avg_gyro_all();
}

void handle_pad(RocketState& state) {}

void update(RocketState& state) {

    switch (state.stage) {
        case FlightStage::Calibrating: 
            handle_calib(state);
        
        case FlightStage::Pad:
            // Later
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
