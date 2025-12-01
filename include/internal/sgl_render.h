#pragma once

#include "sgl_color.h"

namespace sgl::render {
    void set_clear_color(float r, float g, float b, float a) noexcept;

    void set_clear_color(const color &c) noexcept;

    void clear_color_buffer() noexcept;
}
