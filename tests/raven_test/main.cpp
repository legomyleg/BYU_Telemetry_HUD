#include "../../src/hud/run_hud.hpp"
#include "../../src/telemetry/CsvTelemSource.hpp"
#include <string>

int main() {
    std::string path = "/Users/pearsonmorris/Developer/BYU_Telemetry_HUD/data/test_data/test_data.csv";
    auto src = CsvTelemSource(33333, path);
    RunHud(&src, true);
}
