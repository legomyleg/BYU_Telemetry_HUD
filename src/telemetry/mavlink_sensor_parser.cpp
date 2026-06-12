#include <telemetry/mavlink_sensor_parser.hpp>

#include <cstring>

namespace {

uint16_t crc_accumulate(uint8_t byte, uint16_t crc) {
    uint8_t tmp = byte ^ (crc & 0xFF);
    tmp ^= static_cast<uint8_t>(tmp << 4);
    return static_cast<uint16_t>(
        (crc >> 8) ^
        (static_cast<uint16_t>(tmp) << 8) ^
        (static_cast<uint16_t>(tmp) << 3) ^
        (tmp >> 4)
    );
}

uint16_t mavlink_crc(const std::string& frame,
                     size_t start,
                     size_t end,
                     uint8_t crc_extra) {
    uint16_t crc = 0xFFFF;
    for (size_t i = start; i < end; ++i) {
        crc = crc_accumulate(static_cast<uint8_t>(frame[i]), crc);
    }
    return crc_accumulate(crc_extra, crc);
}

uint16_t read_u16_le(const std::string& bytes, size_t offset) {
    return static_cast<uint16_t>(
        static_cast<uint8_t>(bytes[offset]) |
        (static_cast<uint16_t>(static_cast<uint8_t>(bytes[offset + 1])) << 8)
    );
}

uint64_t read_u64_le(const std::string& bytes, size_t offset) {
    uint64_t value = 0;
    for (size_t i = 0; i < 8; ++i) {
        value |= static_cast<uint64_t>(static_cast<uint8_t>(bytes[offset + i])) << (i * 8);
    }
    return value;
}

float read_f32_le(const std::string& bytes, size_t offset) {
    uint32_t raw =
        static_cast<uint8_t>(bytes[offset]) |
        (static_cast<uint32_t>(static_cast<uint8_t>(bytes[offset + 1])) << 8) |
        (static_cast<uint32_t>(static_cast<uint8_t>(bytes[offset + 2])) << 16) |
        (static_cast<uint32_t>(static_cast<uint8_t>(bytes[offset + 3])) << 24);

    float value;
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

uint32_t read_mavlink_msg_id(const std::string& frame) {
    return static_cast<uint8_t>(frame[7]) |
        (static_cast<uint32_t>(static_cast<uint8_t>(frame[8])) << 8) |
        (static_cast<uint32_t>(static_cast<uint8_t>(frame[9])) << 16);
}

SensorData decode_sensor_data_payload(const std::string& payload) {
    SensorData data{};
    data.t_us = read_u64_le(payload, 0);

    float* fields[] = {
        &data.ax,
        &data.ay,
        &data.az,
        &data.gx,
        &data.gy,
        &data.gz,
        &data.mx,
        &data.my,
        &data.mz,
        &data.imuTempC,
        &data.baroTempC,
        &data.pressPa,
        &data.altM,
        &data.hgx,
        &data.hgy,
        &data.hgz,
    };

    for (size_t i = 0; i < 16; ++i) {
        *fields[i] = read_f32_le(payload, 8 + i * sizeof(float));
    }

    return data;
}

}  // namespace

std::vector<SensorData> MavlinkSensorParser::push_bytes(std::string_view bytes) {
    buffer_.append(bytes.data(), bytes.size());

    std::vector<SensorData> samples;
    while (buffer_.size() >= HEADER_LEN) {
        size_t stx_pos = buffer_.find(static_cast<char>(STX));
        if (stx_pos == std::string::npos) {
            buffer_.clear();
            break;
        }

        if (stx_pos > 0) {
            buffer_.erase(0, stx_pos);
            if (buffer_.size() < HEADER_LEN) {
                break;
            }
        }

        size_t payload_len = static_cast<uint8_t>(buffer_[1]);
        uint8_t incompat_flags = static_cast<uint8_t>(buffer_[2]);
        size_t signature_len = (incompat_flags & 0x01) ? SIGNATURE_LEN : 0;
        size_t frame_len = HEADER_LEN + payload_len + CRC_LEN + signature_len;

        if (buffer_.size() < frame_len) {
            break;
        }

        std::string frame = buffer_.substr(0, frame_len);
        std::optional<SensorData> sample = parse_one_frame(frame, payload_len);
        if (sample.has_value()) {
            samples.push_back(*sample);
            buffer_.erase(0, frame_len);
        } else {
            buffer_.erase(0, 1);
        }
    }

    return samples;
}

std::optional<SensorData> MavlinkSensorParser::push_byte(uint8_t byte) {
    char c = static_cast<char>(byte);
    std::vector<SensorData> samples = push_bytes(std::string_view(&c, 1));
    if (samples.empty()) {
        return std::nullopt;
    }
    return samples.front();
}

void MavlinkSensorParser::reset() {
    buffer_.clear();
    stats_ = {};
}

const MavlinkSensorParser::Stats& MavlinkSensorParser::stats() const {
    return stats_;
}

std::optional<SensorData> MavlinkSensorParser::parse_one_frame(const std::string& frame,
                                                               size_t payload_len) {
    uint32_t msg_id = read_mavlink_msg_id(frame);
    if (msg_id != SENSOR_DATA_MSG_ID) {
        ++stats_.frames_ignored;
        return std::nullopt;
    }

    if (payload_len > SENSOR_DATA_PAYLOAD_LEN) {
        ++stats_.frames_len;
        return std::nullopt;
    }

    size_t crc_offset = HEADER_LEN + payload_len;
    uint16_t crc_rx = read_u16_le(frame, crc_offset);
    uint16_t crc_calc = mavlink_crc(frame, 1, crc_offset, SENSOR_DATA_CRC_EXTRA);
    if (crc_rx != crc_calc) {
        ++stats_.frames_crc;
        return std::nullopt;
    }

    std::string payload = frame.substr(HEADER_LEN, payload_len);
    if (payload.size() < SENSOR_DATA_PAYLOAD_LEN) {
        payload.resize(SENSOR_DATA_PAYLOAD_LEN, '\0');
    }

    ++stats_.frames_ok;
    return decode_sensor_data_payload(payload);
}

