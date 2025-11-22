#pragma once

#include "sgl_color.h"

namespace sgl {
    class render_t {
    public:
        static void set_clear_color(float r, float g, float b, float a) noexcept;
        static void set_clear_color(const color_t &color) noexcept;

        static void clear_color_buffer() noexcept;
    };
}
