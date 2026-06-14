#include "telemetry/feed/rtsp_receiver.hpp"
#include <cstdint>
#include <hud/run_hud.hpp>
#include <hud/hud_app.hpp>
#include <state/rocket_state.hpp>
#include <hud/draw/home_window.hpp>
#include <telemetry/telemetry_config.hpp>
#include <logging/data_logger.hpp>

int main() {
    SerialPort serial = SerialPort(PORT_NAME, BAUD_RATE);

    uint64_t buf_size = 3'000'000;

    CameraFeedConfig config = {
        FeedType::Rtsp,
        RTSP_URL
    };
    RunHud(&serial, buf_size, false, true, config);
}
