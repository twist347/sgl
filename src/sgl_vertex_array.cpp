#include "internal/sgl_vertex_array.h"

#include <utility>
#include <cassert>

#include "glad/glad.h"

#include "internal/sgl_log.h"
#include "internal/sgl_util.h"

namespace sgl {
    // ctors and assignments

    vertex_array::vertex_array(vertex_array &&other) noexcept : m_id{std::exchange(other.m_id, 0)} {
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
            log_error("glGenVertexArrays() returned 0");
            return unexpected{error::gl_gen_vertex_arrays_failed};
        }
        return vertex_array{id};
    }

    // try wrappers

    vertex_array vertex_array::create_try() noexcept {
        auto res = create();
        if (!res) {
            log_fatal("failed to create vertex_array: {}", err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void vertex_array::bind() const noexcept {
        assert(m_id);

        glBindVertexArray(m_id);
    }

    void vertex_array::unbind() noexcept {
        glBindVertexArray(0);
    }

    void vertex_array::enable_attrib(gl_uint idx) const noexcept {
        assert(m_id);
        debug_assert_bound(m_id);

        glEnableVertexAttribArray(idx);
    }

    void vertex_array::disable_attrib(gl_uint idx) const noexcept {
        assert(m_id);
        debug_assert_bound(m_id);

        glDisableVertexAttribArray(idx);
    }

    void vertex_array::attrib_pointer(
        gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
    ) const noexcept {
        assert(m_id);
        debug_assert_bound(m_id);

        glVertexAttribPointer(idx, size, type, normalized, stride, pointer);
    }

    void vertex_array::attrib_pointer_and_enable(
        gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
    ) const noexcept {
        attrib_pointer(idx, size, type, normalized, stride, pointer);
        enable_attrib(idx);
    }

    void vertex_array::attrib_pointer_i(
        gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
    ) const noexcept {
        assert(m_id);
        debug_assert_bound(m_id);

        glVertexAttribIPointer(idx, size, type, stride, pointer);
    }

    void vertex_array::attrib_pointer_i_and_enable(
        gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
    ) const noexcept {
        assert(m_id);

        attrib_pointer_i(idx, size, type, stride, pointer);
        enable_attrib(idx);
    }

    void vertex_array::attrib_pointer_l(
        gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
    ) const noexcept {
        assert(m_id);
        debug_assert_bound(m_id);

        glVertexAttribLPointer(idx, size, type, stride, pointer);
    }

    void vertex_array::attrib_pointer_l_and_enable(
        gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
    ) const noexcept {
        assert(m_id);

        attrib_pointer_l(idx, size, type, stride, pointer);
        enable_attrib(idx);
    }

    void vertex_array::debug_assert_bound(gl_uint expected) noexcept {
#ifndef NDEBUG
        gl_int cur = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &cur);
        assert(static_cast<gl_uint>(cur) == expected && "vertex_array: VAO is not bound");
#else
        SGL_UNUSED(expected);
#endif
    }

    // internal

    void vertex_array::destroy() noexcept {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
            m_id = 0;
        }
    }
}
