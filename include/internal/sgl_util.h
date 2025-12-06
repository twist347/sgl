#pragma once

#include <cstddef>

#include "sgl_log.h"

#define SGL_UNUSED(val)    ((void) (val))

#define SGL_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

#define SGL_VERIFY_MSG(cond, msg)                                              \
    do {                                                                       \
        if (!(cond)) {                                                         \
            SGL_LOG_FATAL(                                                     \
                "verify failed: %s. file: %s, line: %d. %s",                   \
                #cond, __FILE__, __LINE__, (msg)                               \
            );                                                                 \
        }                                                                      \
    } while (false)

#define SGL_VERIFY(cond)    SGL_VERIFY_MSG(cond, "")

#define SGL_UNIMPLEMENTED()                                                    \
    do {                                                                       \
        SGL_LOG_FATAL(                                                         \
            "unimplemented: %s (%s:%d)",                                       \
            __func__, __FILE__, __LINE__                                       \
        );                                                                     \
    } while (0)

#define SGL_UNREACHABLE()                                                      \
    do {                                                                       \
        SGL_LOG_FATAL(                                                         \
            "unreachable: %s (%s:%d)",                                         \
            __func__, __FILE__, __LINE__                                       \
        );                                                                     \
    } while (0)

#define SGL_OFFSET_OF(type, member)    (offsetof(type, member))

#define SGL_PTR_OFFSET_OF(type, member)    (reinterpret_cast<void *>(offsetof(type, member)))

#define SGL_SIZEOF_MEMBER(type, member)    (sizeof(((type *)0)->member))

namespace sgl {
    template<typename T, std::size_t N>
    constexpr std::size_t arr_len(T (&)[N]) noexcept {
        return N;
    }
}
