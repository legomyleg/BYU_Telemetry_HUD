#include <cassert>
#include <format>
#include <logging/logger.hpp>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iostream>

using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::time_point_cast;

namespace fs = std::filesystem;

constexpr std::string_view level_to_string(Logger::LOG_LEVEL lvl) {
    switch (lvl) {
        case Logger::LOG_LEVEL::TRACE: return "TRACE";
        case Logger::LOG_LEVEL::DEBUG: return "DEBUG";
        case Logger::LOG_LEVEL::INFO: return "INFO";
        case Logger::LOG_LEVEL::WARN: return "WARN";
        case Logger::LOG_LEVEL::ERROR: return "ERROR";
        case Logger::LOG_LEVEL::CRITICAL: return "CRITICAL";
    }
}

namespace {

    std::ofstream& log_file() {
        
        static std::ofstream file([] {
                fs::path root(ROOT_DIR);
                fs::path log_path = root / "logs";
                if (!fs::exists(log_path)) {
                    fs::create_directory(log_path);
                }

                auto now = time_point_cast<seconds>(system_clock::now());

                auto formatted_path = std::format(
                        "{:%Y-%m-%d_%H-%M-%S}.log", now
                );

                int append_digit = 0;
                while (fs::exists(log_path / formatted_path)) {
                    formatted_path = std::format(
                            "{:%Y-%m-%d_%H-%M-%S}_{}.log", now, append_digit
                    );
                    append_digit++;
                }
                auto new_path = log_path / formatted_path;

                return new_path.string();
        }());

        return file;
    }
}

void Logger::detail::write_log(std::string_view msg, LOG_LEVEL level) {
    if (level < log_level) {
        return;
    }

    std::ofstream& file = log_file();
    auto now = time_point_cast<seconds>(system_clock::now());
    auto time = std::format("{:%H:%M:%S}", now);
    auto log_msg = std::format("{} [{}]: {}", time, level_to_string(level), msg);
    file << log_msg << std::endl;
}
