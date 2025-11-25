#pragma once

namespace sgl::detail::backend {
    bool ensure_glfw() noexcept;

    bool ensure_glad() noexcept;

    bool is_glfw_initialized() noexcept;

    void reset() noexcept;
}
