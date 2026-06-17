#include "hud/run_hud.hpp"
#include "telemetry/feed/rtsp_receiver.hpp"
#include <cstdint>
#include <string>
#include <telemetry/CsvTelemSource.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

std::string JSON_PATH = std::string(ROOT_DIR) + "/include/telemetry/feed/sync.json";
    
struct ReplayInfo {
    std::string video_path;
    std::string telem_data_path;
    double video_sync_time_sec;
    double telem_sync_time_sec;
    double replay_start_before_sync;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReplayInfo, video_path, telem_data_path, video_sync_time_sec, telem_sync_time_sec, replay_start_before_sync);

ReplayInfo get_info() {
    std::ifstream json_file(JSON_PATH);
    
    if (!json_file.is_open()) {
        std::cerr << "Error: Could not open JSON file at " << JSON_PATH << "\n";
        return ReplayInfo{}; // Return a default-initialized struct
    }

    try {
        return nlohmann::json::parse(json_file).get<ReplayInfo>();
    } 
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse error: " << e.what() << "\n";
        return ReplayInfo{}; 
    }
}

int main() {
    ReplayInfo info = get_info();
    double video_start_time = (info.video_sync_time_sec - info.replay_start_before_sync);
    uint64_t telem_start_time_us = (uint64_t)((info.telem_sync_time_sec - info.replay_start_before_sync) * 1'000'000);
    auto tsrc = CsvTelemSource(info.telem_data_path, telem_start_time_us);
    CameraFeedConfig config = {FeedType::VideoFile, info.video_path, video_start_time};

    RunHud(&tsrc, 3'000'000, false, true, config);

    return 0;
}
