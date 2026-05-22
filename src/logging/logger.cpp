#include <logging/logger.hpp>
#include <iostream>
#include <fstream>

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

void Logger::detail::write_log(std::string_view msg, LOG_LEVEL level) {
    
}
