#include "internal/sgl_element_buffer.h"

#include <utility>

#include "glad/glad.h"

#include "internal/sgl_log.h"
#include "internal/sgl_util.h"
#include "internal/sgl_type.h"

namespace sgl {
    const char *element_buffer::err_to_string(error e) noexcept {
        switch (e) {
            case error::INVALID_PARAMS: return "invalid params";
            case error::GL_GEN_FAILED: return "glGenBuffers() failed";
            case error::GL_ALLOC_FAILED: return "glBufferData() failed to allocate";
            default: return "unknown element_buffer_error";
        }
    }

    // ctors and assignments

    element_buffer::element_buffer(element_buffer &&other) noexcept
        : m_id(std::exchange(other.m_id, 0)),
          m_size(std::exchange(other.m_size, 0)),
          m_count(std::exchange(other.m_count, 0)),
          m_type(std::exchange(other.m_type, 0)),
          m_usage(std::exchange(other.m_usage, 0)) {
    }

    element_buffer &element_buffer::operator=(element_buffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy();

        m_id = std::exchange(other.m_id, 0);
        m_size = std::exchange(other.m_size, 0);
        m_count = std::exchange(other.m_count, 0);
        m_type = std::exchange(other.m_type, 0);
        m_usage = std::exchange(other.m_usage, 0);

        return *this;
    }

    element_buffer::~element_buffer() {
        destroy();
    }

    // fabrics

    element_buffer::result element_buffer::create(
        const void *data,
        gl_sizeiptr size,
        gl_enum index_type,
        gl_enum usage
    ) noexcept {
        const gl_sizeiptr idx_size = index_type_size(index_type);
        if (size <= 0 || idx_size == 0 || (size % idx_size) != 0) {
            SGL_LOG_ERROR(
                "element_buffer::create(): invalid params (size=%td, type=0x%x)",
                size, static_cast<unsigned int>(index_type)
            );
            return unexpected{error::INVALID_PARAMS};
        }

        gl_uint id = 0;
        glGenBuffers(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("glGenBuffers() returned 0");
            return unexpected(error::GL_GEN_FAILED);
        }

        GLint prev_vao = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vao);

        glBindVertexArray(0);

        GLint vao0_prev_ebo = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &vao0_prev_ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);

        const bool ok = check_created_size_bound(GL_ELEMENT_ARRAY_BUFFER, size);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao0_prev_ebo);
        glBindVertexArray(prev_vao);

        if (!ok) {
            SGL_LOG_ERROR("glBufferData() failed to allocate %td bytes", size);
            glDeleteBuffers(1, &id);
            return unexpected{error::GL_ALLOC_FAILED};
        }

        const gl_sizei count = size / idx_size;

        return element_buffer{id, size, count, index_type, usage};
    }

    element_buffer element_buffer::create_or_panic(
        const void *data,
        gl_sizeiptr size,
        gl_enum index_type,
        gl_enum usage
    ) noexcept {
        auto res = create(data, size, index_type, usage);
        if (!res) {
            SGL_LOG_FATAL("failed to create element_buffer: %s", err_to_string(res.error()));
        }
        return std::move(*res);
    }

    // api

    void element_buffer::bind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void element_buffer::unbind() noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void element_buffer::set_data(const void *data, gl_sizeiptr size) noexcept {
        if (m_id == 0) {
            SGL_LOG_ERROR("element_buffer::set_data() called on invalid buffer");
            return;
        }

        const gl_sizeiptr idx_size = index_type_size(m_type);
        if (size <= 0 || idx_size == 0 || (size % idx_size) != 0) {
            SGL_LOG_ERROR(
                "element_buffer::set_data(): invalid size=%td for current type=0x%x",
                size, static_cast<unsigned int>(m_type)
            );
            return;
        }

        GLint prev_vao = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prev_vao);

        glBindVertexArray(0);

        GLint vao0_prev_ebo = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &vao0_prev_ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, m_usage);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao0_prev_ebo);
        glBindVertexArray(prev_vao);

        m_size = size;
        m_count = size / idx_size;
    }

    // internal

    gl_sizeiptr element_buffer::index_type_size(gl_enum type) noexcept {
        switch (type) {
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            case GL_UNSIGNED_SHORT: return sizeof(GLushort);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            default: return 0;
        }
    }

    bool element_buffer::check_created_size_bound(gl_enum target, gl_sizeiptr expected) noexcept {
        GLint64 actual = 0;
        glGetBufferParameteri64v(target,GL_BUFFER_SIZE, &actual);
        return actual == expected;
    }

    void element_buffer::destroy() noexcept {
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
            m_size = 0;
            m_count = 0;
            m_type = 0;
            m_usage = 0;
        }
    }
}
