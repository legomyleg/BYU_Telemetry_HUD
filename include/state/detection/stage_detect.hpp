#pragma once
#include "hud/hud_app.hpp"
#include <cstdint>
#include <state/rocket_state.hpp>
#include <telemetry/sample_ring_buffer.hpp>

using microsec = uint64_t;

namespace StageDetect {

    void update(RocketState& state, HudApp& app);

}
