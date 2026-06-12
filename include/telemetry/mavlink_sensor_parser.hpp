#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <telemetry/sensor_data.hpp>

class MavlinkSensorParser {
public:
    struct Stats {
        uint64_t frames_ok = 0;
        uint64_t frames_crc = 0;
        uint64_t frames_len = 0;
        uint64_t frames_ignored = 0;
    };

    std::vector<SensorData> push_bytes(std::string_view bytes);
    std::optional<SensorData> push_byte(uint8_t byte);

    void reset();
    const Stats& stats() const;

private:
    static constexpr uint8_t STX = 0xFD;
    static constexpr size_t HEADER_LEN = 10;
    static constexpr size_t CRC_LEN = 2;
    static constexpr size_t SIGNATURE_LEN = 13;

    static constexpr uint32_t SENSOR_DATA_MSG_ID = 300;
    static constexpr uint8_t SENSOR_DATA_CRC_EXTRA = 95;
    static constexpr size_t SENSOR_DATA_PAYLOAD_LEN = 72;

    std::string buffer_;
    Stats stats_{};

    std::optional<SensorData> parse_one_frame(const std::string& frame,
                                              size_t payload_len);
};

