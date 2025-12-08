#pragma once

#include "sgl_color.h"

namespace sgl::render {
    void set_clear_color(float r, float g, float b, float a) noexcept;

    void set_clear_color(const color &c) noexcept;

    void clear(bool color = true, bool depth = false, bool stencil = false) noexcept;

    void clear_color_buffer() noexcept;

    void clear_color_depth_buffer() noexcept;

    void enable_depth_test(bool enabled) noexcept;

    void enable_blend(bool enabled) noexcept;
}
