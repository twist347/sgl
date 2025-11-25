#include "internal/sgl_vertex_array.h"

#include <utility>

#include "glad/glad.h"

#include "internal/sgl_vertex_buffer.h"
#include "internal/sgl_log.h"
#include "internal/sgl_util.h"
#include "internal/sgl_type.h"

namespace sgl {
    // ctors and assignments

    vertex_array::vertex_array(vertex_array &&other) noexcept : m_id(std::exchange(other.m_id, 0)) {
    }

    vertex_array &vertex_array::operator=(vertex_array &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy();

        m_id = std::exchange(other.m_id, 0);

        return *this;
    }

    vertex_array::~vertex_array() {
        destroy();
    }

    // fabrics

    vertex_array::result vertex_array::create() noexcept {
        gl_uint id = 0;
        glGenVertexArrays(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("glGenVertexArrays() returned 0");
            return unexpected{error::GL_GEN_FAILED};
        }
        return vertex_array{id};
    }

    vertex_array vertex_array::create_or_panic() noexcept {
        auto res = create();
        if (!res) {
            SGL_LOG_FATAL("failed to create vertex_array: %s", err_to_string(res.error()));
        }
        return std::move(*res);
    }

    // api

    void vertex_array::bind() const noexcept {
        glBindVertexArray(m_id);
    }

    void vertex_array::unbind() noexcept {
        glBindVertexArray(0);
    }

    void vertex_array::enable_attrib(gl_uint idx) const noexcept {
        glEnableVertexAttribArray(idx);
    }

    void vertex_array::disable_attrib(gl_uint idx) const noexcept {
        glDisableVertexAttribArray(idx);
    }

    void vertex_array::attrib_pointer_f(
        const vertex_buffer &vbo,
        gl_uint idx,
        gl_int size,
        gl_enum type,
        gl_boolean normalized,
        gl_sizei stride,
        const void *pointer
    ) const noexcept {
        bind();
        vbo.bind();

        glVertexAttribPointer(idx, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(idx);
    }

    void vertex_array::attrib_pointer_i(
        const vertex_buffer &vbo,
        gl_uint idx,
        gl_int size,
        gl_enum type,
        gl_sizei stride,
        const void *pointer
    ) const noexcept {
        bind();
        vbo.bind();

        glVertexAttribIPointer(idx, size, type, stride, pointer);
        glEnableVertexAttribArray(idx);
    }

    void vertex_array::attrib_pointer_l(
        const vertex_buffer &vbo,
        gl_uint idx,
        gl_int size,
        gl_enum type,
        gl_sizei stride,
        const void *pointer
    ) const noexcept {
        bind();
        vbo.bind();

        glVertexAttribLPointer(idx, size, type, stride, pointer);
        glEnableVertexAttribArray(idx);
    }

    const char *vertex_array::err_to_string(error e) noexcept {
        switch (e) {
            case error::GL_GEN_FAILED: return "glGenVertexArrays() failed";
            default: return "unknown vertex_array_error";
        }
    }

    // internal

    void vertex_array::destroy() noexcept {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
            m_id = 0;
        }
    }
}
