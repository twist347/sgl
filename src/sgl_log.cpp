#include "internal/sgl_log.h"

#include <array>

namespace sgl::detail {
    const char *level_name(log_level_e level) noexcept {
        static constexpr std::array<const char *, static_cast<std::size_t>(log_level_e::COUNT)> names{
            "INFO",
            "WARNING",
            "ERROR",
            "FATAL"
        };
        return names[static_cast<std::size_t>(level)];
    }

    FILE *stream_for(log_level_e level) noexcept {
        switch (level) {
            case log_level_e::INFO:
            case log_level_e::WARNING:
                return stdout;
            case log_level_e::ERROR:
            case log_level_e::FATAL:
            default:
                return stderr;
        }
    }
}
