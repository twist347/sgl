#pragma once

#include "sgl_log.h"

#if !defined(__has_include)
  #error "This header requires __has_include (C++17+)."
#endif

#if __has_include(<version>)
  #include <version> // for __cpp_lib_* macro
#endif

#if __has_include(<expected>) && defined(__cpp_lib_expected)
    #include <expected>
    namespace ns_exp = std;
#elif __has_include(<expected.hpp>)
    #include <expected.hpp>
    namespace ns_exp = tl;
#else
    #error "No <expected> or <expected.hpp> found."
#endif

namespace sgl {
    template<typename T, typename E>
    using expected = ns_exp::expected<T, E>;

    template<typename E>
    using unexpected = ns_exp::unexpected<E>;

    using unexpect_t = ns_exp::unexpect_t;

    inline constexpr unexpect_t unexpect = ns_exp::unexpect;

    template<typename E>
    using bad_expected_access = ns_exp::bad_expected_access<E>;

    template<typename T, typename E>
    T unwrap(expected<T, E> e, const char *msg = "") noexcept {
        if (!e) {
            log_fatal("{}", msg);
            return {};
        }
        return std::move(*e);
    }
}
