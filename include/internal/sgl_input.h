#pragma once

#include "sgl_key.h"

namespace sgl::input {
    bool is_key_down(key k) noexcept;
    bool is_key_up(key k) noexcept;

    bool is_key_pressed(key k) noexcept;
    bool is_key_released(key k) noexcept;
}


namespace sgl::detail::input {
    void on_key(int glfw_key, int action) noexcept;

    void new_frame() noexcept;
}
