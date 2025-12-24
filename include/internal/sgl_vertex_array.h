#pragma once

#include "sgl_expected.h"
#include "sgl_type.h"
#include "sgl_util.h"

namespace sgl {
    enum class vertex_array_error {
        gl_gen_vertex_arrays_failed = 0,
        count
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

        // try wrappers

        static vertex_array create_try() noexcept;

        // api

        void bind() const noexcept;

        static void unbind() noexcept;

        [[nodiscard]] gl_uint id() const noexcept { return m_id; }

        static void enable_attrib(gl_uint idx) noexcept;

        static void disable_attrib(gl_uint idx) noexcept;

        void attrib_pointer(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
        ) const noexcept;

        template<typename Vertex, typename Member>
        void attrib_pointer(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, Member Vertex::*member
        ) const noexcept {
            const std::size_t off = offset_of(member);
            const auto ptr = reinterpret_cast<const void *>(static_cast<std::uintptr_t>(off));
            attrib_pointer(idx, size, type, normalized, sizeof(Vertex), ptr);
        }

        template<typename Vertex, typename Member>
        void attrib_pointer_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, Member Vertex::*member
        ) const noexcept {
            const std::size_t off = offset_of(member);
            const auto ptr = reinterpret_cast<const void *>(static_cast<std::uintptr_t>(off));
            attrib_pointer_and_enable(idx, size, type, normalized, sizeof(Vertex), ptr);
        }

        void attrib_pointer_i(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_i_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_l(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_l_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        inline constexpr static const char *err_to_str(error e) noexcept {
            switch (e) {
                case error::gl_gen_vertex_arrays_failed: return "glGenVertexArrays() failed";
                default: return "unknown vertex_array_error";
            }
        }

    private:
        explicit vertex_array(gl_uint id) noexcept : m_id{id} {
        }

        void destroy() noexcept;

        gl_uint m_id = 0;
    };
}
