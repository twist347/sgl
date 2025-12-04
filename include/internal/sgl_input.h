#pragma once

#include "sgl_key.h"

namespace sgl::input {
    bool is_key_down(key k) noexcept;
    bool is_key_up(key k) noexcept;

    bool is_key_pressed(key k) noexcept;
    bool is_key_released(key k) noexcept;

    bool is_mouse_button_down(mouse_button b) noexcept;
    bool is_mouse_button_up(mouse_button b) noexcept;

    bool is_mouse_button_pressed(mouse_button b) noexcept;
    bool is_mouse_button_released(mouse_button b) noexcept;

    void mouse_position(double &x, double &y) noexcept;

    void mouse_delta(double &dx, double &dy) noexcept;

    void scroll_delta(double &dx, double &dy) noexcept;
}

namespace sgl::detail::input {
    void on_key(int glfw_key, int action) noexcept;

    void on_mouse_button(int button, int action) noexcept;
    void on_cursor_pos(double x, double y) noexcept;
    void on_scroll(double x_offset, double y_offset) noexcept;

    void new_frame() noexcept;
}
