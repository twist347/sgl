#include "internal/sgl_log.h"

#include <array>
#include <cstddef>

namespace sgl::detail {
    const char *level_name(log_level level) noexcept {
        static constexpr std::array<const char *, static_cast<std::size_t>(log_level::count)> names{
            "INFO",
            "WARNING",
            "ERROR",
            "FATAL"
        };
        static_assert(names.size() == static_cast<std::size_t>(log_level::count));
        return names[static_cast<std::size_t>(level)];
    }

    FILE *stream_for(log_level level) noexcept {
        switch (level) {
            case log_level::info:
            case log_level::warn:
                return stdout;
            case log_level::error:
            case log_level::fatal:
            default:
                return stderr;
        }
    }
}
