#include "internal/sgl_render.h"

#include "glad/glad.h"

namespace sgl {

    void render::set_clear_color(float r, float g, float b, float a) noexcept {
        glClearColor(r, g, b, a);
    }

    void render::set_clear_color(const color &c) noexcept {
        glClearColor(c.r, c.g, c.b, c.a);
    }

    void render::clear_color_buffer() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

}
