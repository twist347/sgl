#pragma once

#include <type_traits>

#include "sgl_expected.h"
#include "sgl_type.h"

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

        void enable_attrib(gl_uint idx) const noexcept;

        void disable_attrib(gl_uint idx) const noexcept;

        void attrib_pointer(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, gl_sizei stride, const void *pointer
        ) const noexcept;

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, const void *pointer
        ) const noexcept {
            attrib_pointer(idx, size, type, normalized, sizeof(Vertex), pointer);
        }

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_boolean normalized, const void *pointer
        ) const noexcept {
            attrib_pointer_and_enable(idx, size, type, normalized, sizeof(Vertex), pointer);
        }

        void attrib_pointer_i(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_i_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer_i(
            gl_uint idx, gl_int size, gl_enum type, const void *pointer
        ) const noexcept {
            attrib_pointer_i(idx, size, type, sizeof(Vertex), pointer);
        }

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer_i_and_enable(
            gl_uint idx, gl_int size, gl_enum type, const void *pointer
        ) const noexcept {
            attrib_pointer_i_and_enable(idx, size, type, sizeof(Vertex), pointer);
        }

        void attrib_pointer_l(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        void attrib_pointer_l_and_enable(
            gl_uint idx, gl_int size, gl_enum type, gl_sizei stride, const void *pointer
        ) const noexcept;

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer_l(
            gl_uint idx, gl_int size, gl_enum type, const void *pointer
        ) const noexcept {
            attrib_pointer_l(idx, size, type, sizeof(Vertex), pointer);
        }

        template<typename Vertex>
            requires std::is_trivially_copyable_v<Vertex>
        void attrib_pointer_l_and_enable(
            gl_uint idx, gl_int size, gl_enum type, const void *pointer
        ) const noexcept {
            attrib_pointer_l_and_enable(idx, size, type, sizeof(Vertex), pointer);
        }

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

        static void debug_assert_bound(gl_uint expected) noexcept;

        gl_uint m_id = 0;
    };
}
