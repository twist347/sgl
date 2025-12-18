#include "internal/sgl_vertex_buffer.h"

#include <utility>
#include <cassert>

#include "glad/glad.h"

#include "internal/sgl_log.h"

namespace sgl {
    // ctors and assignments

    vertex_buffer::vertex_buffer(vertex_buffer &&other) noexcept : m_id{std::exchange(other.m_id, 0)},
                                                                   m_size{std::exchange(other.m_size, 0)},
                                                                   m_usage{std::exchange(other.m_usage, 0)} {
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
            return unexpected{error::invalid_params};
        }

        gl_uint id = 0;
        glGenBuffers(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("glGenBuffers() returned 0");
            return unexpected{error::gl_gen_buffers_failed};
        }

        gl_int prev = 0;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prev);

        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);

        const bool ok = check_created_size_bound(GL_ARRAY_BUFFER, size);

        glBindBuffer(GL_ARRAY_BUFFER, prev);

        if (!ok) {
            SGL_LOG_ERROR("glBufferData() failed to allocate %td bytes", size);
            glDeleteBuffers(1, &id);
            return unexpected{error::gl_alloc_failed};
        }

        return vertex_buffer{id, size, usage};
    }

    vertex_buffer vertex_buffer::create_try(const void *data, gl_sizeiptr size, gl_enum usage) noexcept {
        auto res = create(data, size, usage);
        if (!res) {
            SGL_LOG_FATAL("failed to create vertex_buffer: %s", err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void vertex_buffer::bind() const noexcept {
        assert(m_id != 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void vertex_buffer::unbind() noexcept {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void vertex_buffer::set_data(const void *data, gl_sizeiptr size) const noexcept {
        assert(m_id != 0);
        assert(size > 0);

        glBufferData(GL_ARRAY_BUFFER, size, data, m_usage);
    }

    // internal

    bool vertex_buffer::check_created_size_bound(gl_enum target, gl_sizeiptr expected) noexcept {
        GLint64 actual = 0;
        glGetBufferParameteri64v(target, GL_BUFFER_SIZE, &actual);
        return actual == expected;
    }

    constexpr const char *vertex_buffer::err_to_str(error e) noexcept {
        switch (e) {
            case error::invalid_params: return "invalid params";
            case error::gl_gen_buffers_failed: return "glGenBuffers() failed";
            case error::gl_alloc_failed: return "glBufferData() failed to allocate";
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
