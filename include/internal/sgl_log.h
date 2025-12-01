#pragma once

#include <cstdlib>
#include <utility>
#include <cstdio>

#define SGL_LOG_INFO(fmt, ...)  ::sgl::log(::sgl::log_level_e::INFO,   (fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_WARN(fmt, ...)  ::sgl::log(::sgl::log_level_e::WARNING,(fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_ERROR(fmt, ...) ::sgl::log(::sgl::log_level_e::ERROR,  (fmt) __VA_OPT__(,) __VA_ARGS__)
#define SGL_LOG_FATAL(fmt, ...) ::sgl::log(::sgl::log_level_e::FATAL,  (fmt) __VA_OPT__(,) __VA_ARGS__)

namespace sgl {
    enum class log_level_e {
        INFO = 0,
        WARNING,
        ERROR,
        FATAL,
        COUNT
    };

    namespace detail {
        const char *level_name(log_level_e level) noexcept;

        FILE *stream_for(log_level_e level) noexcept;
    }

    template<typename... Args>
    void log(log_level_e level, const char *fmt, Args &&... args) noexcept {
        FILE *out = detail::stream_for(level);

        std::fprintf(out, "[%s]: ", detail::level_name(level));

        if constexpr (sizeof...(Args) == 0) {
            std::fputs(fmt, out);
        } else {
            std::fprintf(out, fmt, std::forward<Args>(args)...);
        }

        std::fprintf(out, "\n");
        std::fflush(out);

        if (level == log_level_e::FATAL) {
            std::exit(EXIT_FAILURE);
        }
    }
}
