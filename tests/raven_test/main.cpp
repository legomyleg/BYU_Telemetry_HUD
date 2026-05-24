#include <hud/run_hud.hpp>
#include <telemetry/CsvTelemSource.hpp>
#include <telemetry/telemetry_config.hpp>
#include <chrono>
#include <string>
#include <logging/logger.hpp>

int main() {

    Logger::log_level = Logger::LOG_LEVEL::TRACE;

    LOG_TRACE("Entering main.cpp");

    std::string path = "/Users/pearsonmorris/Developer/BYU_Telemetry_HUD/data/test_data/test_data.csv";
    auto src = CsvTelemSource(33333, path);

    auto bufsize = microseconds(TWO_SECOND);
    RunHud(&src, bufsize, true);

    LOG_TRACE("Exiting main.cpp");
}
