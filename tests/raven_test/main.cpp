#include <hud/run_hud.hpp>
#include <telemetry/CsvTelemSource.hpp>
#include <telemetry/telemetry_config.hpp>
#include <logging/logger.hpp>

int main() {

    Logger::log_level = Logger::LOG_LEVEL::TRACE;

    LOG_TRACE("Entering main.cpp");

    std::string path = "/Users/pearsonmorris/Developer/BYU_HUD_Developement/BYU_Telemetry_HUD/data/test_data/new_test_data.csv";
    auto src = CsvTelemSource(33333, path);

    RunHud(&src, 1'000'000);

    LOG_TRACE("Exiting main.cpp");
}
