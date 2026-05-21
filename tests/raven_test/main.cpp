#include <hud/run_hud.hpp>
#include <telemetry/CsvTelemSource.hpp>
#include <telemetry/telemetry_config.hpp>
#include <chrono>
#include <string>

int main() {
    std::string path = "/Users/pearsonmorris/Developer/BYU_Telemetry_HUD/data/test_data/test_data.csv";
    auto src = CsvTelemSource(33333, path);

    auto bufsize = microseconds(TWO_SECOND);
    RunHud(&src, bufsize, true);
}
