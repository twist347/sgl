#pragma once

#include "sgl_expected.h"
#include "sgl_type.h"

namespace sgl {
    enum class element_buffer_error {
        invalid_params = 0,
        gl_gen_buffers_failed,
        gl_alloc_failed,
        count
    };

    class element_buffer {
    public:
        using error = element_buffer_error;
        using result = expected<element_buffer, error>;

        // ctors and assignments

        element_buffer(const element_buffer &) = delete;

        element_buffer &operator=(const element_buffer &) = delete;

        element_buffer(element_buffer &&other) noexcept;

        element_buffer &operator=(element_buffer &&other) noexcept;

        ~element_buffer();

        // fabrics

        static result create(const void *data, gl_sizeiptr size, gl_enum index_type, gl_enum usage) noexcept;

        // try wrappers

        static element_buffer create_try(
            const void *data, gl_sizeiptr size, gl_enum index_type, gl_enum usage
        ) noexcept;

        // api

        void bind() const noexcept;

        static void unbind_current_vao() noexcept;

        void set_data(const void *data, gl_sizeiptr size) noexcept;

        [[nodiscard]] gl_uint id() const noexcept { return m_id; }
        [[nodiscard]] gl_sizeiptr size() const noexcept { return m_size; }
        [[nodiscard]] gl_sizei count() const noexcept { return m_count; }
        [[nodiscard]] gl_enum type() const noexcept { return m_type; }
        [[nodiscard]] gl_enum usage() const noexcept { return m_usage; }

        inline constexpr static const char *err_to_str(error e) noexcept {
            switch (e) {
                case error::invalid_params: return "invalid params";
                case error::gl_gen_buffers_failed: return "glGenBuffers() failed";
                case error::gl_alloc_failed: return "glBufferData() failed to allocate";
                default: return "unknown element_buffer_error";
            }
        }

    private:
        constexpr static gl_sizeiptr index_type_size(gl_enum type) noexcept;

        static bool check_created_size_bound(gl_enum target, gl_sizeiptr expected) noexcept;

    private:
        explicit element_buffer(gl_uint id, gl_sizeiptr size, gl_sizei count, gl_enum type, gl_enum usage) noexcept
            : m_id{id}, m_size{size}, m_count{count}, m_type{type}, m_usage{usage} {
        }

        void destroy() noexcept;

        gl_uint m_id = 0;
        gl_sizeiptr m_size = 0;
        gl_sizei m_count = 0;
        gl_enum m_type = 0;
        gl_enum m_usage = 0;
    };
}
