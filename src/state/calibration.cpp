#include "calibration.hpp"
#include "hud/hud_app.hpp"
#include "state/rocket_state.hpp"
#include "state/state_update.hpp"
#include "telemetry/serial.hpp"
#include <cassert>

void calibrate(HudApp &app, SerialPort &serial) {
    assert(app.state.sampleWindow.empty());

    while(!app.state.sampleWindow.full()) {
        ReadSerialSamples(app, serial);
    }

    app.biases = {app.state.sampleWindow.avg_accel(), app.state.sampleWindow.avg_gyro()};

}

