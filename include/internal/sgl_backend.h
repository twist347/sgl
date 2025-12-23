#pragma once

namespace sgl::detail {
    bool ensure_glfw() noexcept;

    bool ensure_glad() noexcept;

    bool is_glfw_initialized() noexcept;

    bool is_glad_initialized() noexcept;

    void reset() noexcept;
}
