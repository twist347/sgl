#pragma once

#include "sgl_type.h"
#include "sgl_expected.h"

namespace sgl {
    enum class vertex_buffer_error {
        invalid_params = 0,
        gl_gen_buffers_failed,
        gl_alloc_failed,
        count
    };

    class vertex_buffer {
    public:
        using error = vertex_buffer_error;
        using result = expected<vertex_buffer, error>;

        // ctors and assignments

        vertex_buffer(const vertex_buffer &) = delete;

        vertex_buffer &operator=(const vertex_buffer &) = delete;

        vertex_buffer(vertex_buffer &&other) noexcept;

        vertex_buffer &operator=(vertex_buffer &&other) noexcept;

        ~vertex_buffer();

        // fabrics

        static result create(const void *data, gl_sizeiptr size, gl_enum usage) noexcept;

        // or panic wrapper

        static vertex_buffer create_try(const void *data, gl_sizeiptr size, gl_enum usage) noexcept;

        // api

        void bind() const noexcept;

        static void unbind() noexcept;

        void set_data(const void *data, gl_sizeiptr size) const noexcept;

        [[nodiscard]] gl_uint get_id() const noexcept { return m_id; }
        [[nodiscard]] gl_sizeiptr get_size() const noexcept { return m_size; }
        [[nodiscard]] gl_enum get_usage() const noexcept { return m_usage; }

        constexpr static const char *err_to_str(error e) noexcept;

    private:
        static bool check_created_size_bound(gl_enum target, gl_sizeiptr expected) noexcept;

    private:
        explicit vertex_buffer(gl_uint id, gl_sizeiptr size, gl_enum usage) noexcept
            : m_id{id}, m_size{size}, m_usage{usage} {
        }

        void destroy() noexcept;

        gl_uint m_id = 0;
        gl_sizeiptr m_size = 0;
        gl_enum m_usage = 0;
    };
}
