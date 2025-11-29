#include "internal/sgl_render.h"

#include "glad/glad.h"
#include "internal/sgl_element_buffer.h"

#include "internal/sgl_vertex_array.h"
#include "internal/sgl_shader.h"

namespace sgl::render {
    void set_clear_color(float r, float g, float b, float a) noexcept {
        glClearColor(r, g, b, a);
    }

    void set_clear_color(const color &c) noexcept {
        glClearColor(
            static_cast<float>(c.r) / 255.0f,
            static_cast<float>(c.g) / 255.0f,
            static_cast<float>(c.b) / 255.0f,
            static_cast<float>(c.a) / 255.0f
        );
    }

    void clear_color_buffer() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
