#pragma once

#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cassert>

#include "fmt/base.h"

namespace sgl {
    enum class log_level {
        info = 0,
        warn,
        error,
        fatal,

        count
    };

    namespace detail {
        const char *level_name(log_level level) noexcept;

        FILE *stream_for(log_level level) noexcept;
    }

    template<typename... Args>
    void log(log_level level, fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        assert(level < log_level::count);

        FILE *out = detail::stream_for(level);
        try {
            fmt::print(out, "[{}]: ", detail::level_name(level));
            fmt::println(out, fmt, std::forward<Args>(args)...);
        } catch (...) {
            std::fputs("[LOG]: formatting failed", stderr);
        }
        if (level == log_level::fatal) {
            std::abort();
        }
    }

    template<typename... Args>
    void log_info(fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        log(log_level::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_warn(fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        log(log_level::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_error(fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        log(log_level::error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log_fatal(fmt::format_string<Args...> fmt, Args &&... args) noexcept {
        log(log_level::fatal, fmt, std::forward<Args>(args)...);
    }
}
