#include <cassert>
#include <format>
#include <logging/logger.hpp>
#include <fstream>
#include <filesystem>
#include <chrono>

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

                auto now = std::chrono::system_clock::now();

                fs::path new_path = log_path / std::format(
                        "{:%Y-%m-%d_%H-%M-%S}.log", now
                );
                assert(!fs::exists(new_path));

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
    auto now = std::chrono::system_clock::now();
    auto time = std::format("{:%H-%M-%S}", now);
    auto log_msg = std::format("{} [{}]: {}\n", time, level_to_string(level), msg);
    file << log_msg;
}
