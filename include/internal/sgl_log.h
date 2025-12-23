#pragma once

#include <cstdlib>
#include <utility>
#include <cstdio>

#define SGL_LOG_INFO(fmt, ...)  ::sgl::log(::sgl::log_level::info,   (fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_WARN(fmt, ...)  ::sgl::log(::sgl::log_level::warn,(fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_ERROR(fmt, ...) ::sgl::log(::sgl::log_level::error,  (fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_FATAL(fmt, ...) ::sgl::log(::sgl::log_level::fatal,  (fmt) __VA_OPT__(,) __VA_ARGS__)

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
    void log(log_level level, const char *fmt, Args &&... args) noexcept {
        FILE *out = detail::stream_for(level);

        std::fprintf(out, "[%s]: ", detail::level_name(level));

        if constexpr (sizeof...(Args) == 0) {
            std::fputs(fmt, out);
        } else {
            std::fprintf(out, fmt, std::forward<Args>(args)...);
        }

        std::fprintf(out, "\n");

        if (level == log_level::fatal) {
            std::fflush(out);
            std::fflush(stdout);
            std::fflush(stderr);
            std::abort();
        }
    }
}
