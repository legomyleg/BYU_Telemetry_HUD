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

    inline bool enabled = false;
    inline LOG_LEVEL log_level = LOG_LEVEL::ERROR;
};

namespace Logger {
    namespace detail {
        void write_log(std::string_view msg, LOG_LEVEL level);
    }
}

#define LOG(msg, level) \
    do { \
        if (Logger::enabled) { \
            Logger::detail::write_log(msg, level); \
        } \
    } while (0)
