#include <string_view>

namespace Logger {

    enum class LOG_LEVEL {
        TRACE = 10,
        DEBUG = 20,
        INFO = 30,
        WARN = 40,
        ERROR = 50,
        CRITICAL = 60
    };

    inline LOG_LEVEL log_level = LOG_LEVEL::WARN;
};

namespace Logger {
    namespace detail {
        void write_log(std::string_view msg, LOG_LEVEL level);
    }
}

// SOME MACROS TO DEFINE LOGGING
#if defined(ENABLE_LOGGING) && ENABLE_LOGGING
#define LOG_TRACE(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::TRACE); \
    } while (0)
#define LOG_DEBUG(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::DEBUG); \
    } while (0)
#define LOG_INFO(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::INFO); \
    } while (0)
#define LOG_WARN(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::WARN); \
    } while (0)
#define LOG_ERROR(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::ERROR); \
    } while (0)
#define LOG_CRITICAL(msg) \
    do { \
        Logger::detail::write_log(msg, Logger::LOG_LEVEL::CRITICAL); \
    } while (0)
#else
#define LOG_TRACE(msg) do {} while (0)
#define LOG_DEBUG(msg) do {} while (0)
#define LOG_INFO(msg) do {} while (0)
#define LOG_WARN(msg) do {} while (0)
#define LOG_ERROR(msg) do {} while (0)
#define LOG_CRITICAL(msg) do {} while (0)
#endif
