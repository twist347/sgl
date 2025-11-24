#include "internal/sgl_vertex_buffer.h"

#include <utility>

#include "glad/glad.h"

#include "internal/sgl_log.h"

namespace sgl {
    // ctors and assignments

    vertex_buffer::vertex_buffer(vertex_buffer &&other) noexcept : m_id{std::exchange(other.m_id, 0)},
                                                                   m_size(std::exchange(other.m_size, 0)),
                                                                   m_usage(std::exchange(other.m_usage, 0)) {
    }

    vertex_buffer &vertex_buffer::operator=(vertex_buffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy();

        m_id = std::exchange(other.m_id, 0);
        m_size = std::exchange(other.m_size, 0);
        m_usage = std::exchange(other.m_usage, 0);

        return *this;
    }

    vertex_buffer::~vertex_buffer() {
        destroy();
    }

    // fabrics

    vertex_buffer::result vertex_buffer::create(const void *data, gl_sizeiptr size, gl_enum usage) noexcept {
        if (size <= 0) {
            return unexpected(error::INVALID_PARAMS);
        }

        gl_uint id = 0;
        glGenBuffers(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("glGenBuffers() returned 0");
            return unexpected(error::GL_GEN_FAILED);
        }

        GLint prev = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev);

        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);

        const bool ok = check_created_size_bound(GL_ARRAY_BUFFER, size);

        glBindBuffer(GL_ARRAY_BUFFER, prev);

        if (!ok) {
            SGL_LOG_ERROR("glBufferData() failed to allocate %td bytes", size);
            glDeleteBuffers(1, &id);
            return unexpected(error::GL_ALLOC_FAILED);
        }

        return vertex_buffer{id, size, usage};
    }

    vertex_buffer vertex_buffer::create_or_panic(const void *data, gl_sizeiptr size, gl_enum usage) noexcept {
        auto res = create(data, size, usage);
        if (!res) {
            SGL_LOG_FATAL("failed to create vertex_buffer: %s", err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void vertex_buffer::bind() const noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void vertex_buffer::unbind() noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void vertex_buffer::set_data(const void *data, gl_sizeiptr size) noexcept {
        if (m_id == 0) {
            SGL_LOG_ERROR("vertex_buffer::set_data() called on invalid buffer");
            return;
        }

        GLint prev = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev);

        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
        glBindBuffer(GL_ARRAY_BUFFER, prev);

        m_size = size;
    }

    // internal

    bool vertex_buffer::check_created_size_bound(gl_enum target, gl_sizeiptr expected) {
        GLint64 actual = 0;
        glGetBufferParameteri64v(target, GL_BUFFER_SIZE, &actual);
        return actual == expected;
    }

    const char *vertex_buffer::err_to_str(error e) noexcept {
        switch (e) {
            case error::INVALID_PARAMS: return "invalid params";
            case error::GL_GEN_FAILED: return "glGenBuffers() failed";
            case error::GL_ALLOC_FAILED: return "glBufferData() failed to allocate";
            default: return "unknown vertex_buffer_error";
        }
    }

    void vertex_buffer::destroy() noexcept {
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
            m_size = 0;
        }
    }
}
