#pragma once

#include "sgl_expected.h"
#include "internal/sgl_type.h"

namespace sgl {
    class vertex_buffer;

    enum class vertex_array_error {
        GL_GEN_FAILED = 0,
        COUNT
    };

    class vertex_array {
    public:
        using error = vertex_array_error;
        using result = expected<vertex_array, error>;

        // ctors and assignments

        vertex_array(const vertex_array &) = delete;

        vertex_array &operator=(const vertex_array &) = delete;

        vertex_array(vertex_array &&other) noexcept;

        vertex_array &operator=(vertex_array &&other) noexcept;

        ~vertex_array();

        // fabrics
        static result create() noexcept;

        // or panic wrapper

        static vertex_array create_or_panic() noexcept;

        // api

        void bind() const noexcept;

        static void unbind() noexcept;

        [[nodiscard]] gl_uint id() const noexcept { return m_id; }

        void enable_attrib(gl_uint idx) const noexcept;

        void disable_attrib(gl_uint idx) const noexcept;

        void attrib_pointer_f(
            const vertex_buffer &vbo,
            gl_uint idx,
            gl_int size,
            gl_enum type,
            gl_boolean normalized,
            gl_sizei stride,
            const void *pointer
        ) const noexcept;

        void attrib_pointer_i(
            const vertex_buffer &vbo,
            gl_uint idx,
            gl_int size,
            gl_enum type,
            gl_sizei stride,
            const void *pointer
        ) const noexcept;

        void attrib_pointer_l(
            const vertex_buffer &vbo,
            gl_uint idx,
            gl_int size,
            gl_enum type,
            gl_sizei stride,
            const void *pointer
        ) const noexcept;

        static const char *err_to_string(error e) noexcept;

    private:
        explicit vertex_array(gl_uint id) noexcept : m_id(id) {
        }

        void destroy() noexcept;

        gl_uint m_id = 0;
    };
} // namespace sgl
