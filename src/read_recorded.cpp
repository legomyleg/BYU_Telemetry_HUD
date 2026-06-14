#include "hud/run_hud.hpp"
#include <telemetry/CsvTelemSource.hpp>

int main() {
    auto tsrc = CsvTelemSource(std::string(ROOT_DIR) + "/data/logged_data/data_log.csv");

    RunHud(&tsrc, 3'000'000, false, false);

    return 0;
}
