#pragma once
#include <cstdint>
#include <state/rocket_state.hpp>
#include <telemetry/sample_ring_buffer.hpp>

using microsec = uint64_t;

struct StageDetector {
    FlightStage stage;
    microsec stage_start_us = 0;

    void transition_to(FlightStage next, microsec now_us) {
        stage = next;
        stage_start_us = now_us;
    }
};

void update(RocketState& state);
