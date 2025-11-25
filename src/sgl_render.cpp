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

    void draw_arrays(
        gl_enum mode,
        const shader &shader,
        const vertex_array &vao,
        gl_int first,
        gl_sizei count
    ) noexcept {
        shader.use();
        vao.bind();

        glDrawArrays(mode, first, count);
        vertex_array::unbind();
    }

    void draw_elements(
        gl_enum mode,
        const shader &shader,
        const vertex_array &vao,
        const element_buffer &ebo
    ) noexcept {
        shader.use();
        vao.bind();

        glDrawElements(mode, ebo.count(), ebo.type(), nullptr);

        vertex_array::unbind();
    }
}
