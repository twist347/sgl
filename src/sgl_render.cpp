#include "internal/sgl_render.h"

#include "glad/glad.h"

namespace sgl {

    void render_t::set_clear_color(float r, float g, float b, float a) noexcept {
        glClearColor(r, g, b, a);
    }

    void render_t::set_clear_color(const color_t &color) noexcept {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void render_t::clear_color_buffer() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

}
