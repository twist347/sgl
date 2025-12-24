#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

#include "sgl_log.h"

namespace sgl {
    template<typename T, std::size_t N>
    constexpr std::size_t arr_len(T (&)[N]) noexcept {
        return N;
    }

    template<typename... Args>
    constexpr void unused(Args &&...) noexcept {
    }

    template<typename T, typename M>
    constexpr std::size_t sizeof_member(M T::*) noexcept {
        return sizeof(M);
    }

    template<typename T, typename M>
        requires (std::is_standard_layout_v<T> && std::is_default_constructible_v<T>)
    constexpr std::size_t offset_of(M T::*member) noexcept(std::is_nothrow_default_constructible_v<T>) {
        T obj{};
        const auto base = reinterpret_cast<const std::byte *>(std::addressof(obj));
        const auto mem = reinterpret_cast<const std::byte *>(std::addressof(obj.*member));
        return static_cast<std::size_t>(mem - base);
    }

    namespace detail {
        template<typename T>
            requires std::is_standard_layout_v<T>
        constexpr std::size_t checked_offset(std::size_t off) noexcept {
            return off;
        }
    }
}

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

#define SGL_OFFSET_OF(type, member)    (::sgl::detail::checked_offset<type>(offsetof(type, member)))

#define SGL_PTR_OFFSET_OF(type, member)    (reinterpret_cast<void *>(SGL_OFFSET_OF(type, member)))

#define SGL_SIZEOF_MEMBER(type, member)    sizeof(std::declval<type>().member)
