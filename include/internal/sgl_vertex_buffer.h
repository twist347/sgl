#pragma once

#include <type_traits>
#include <span>

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

        template<class T, std::size_t Extent>
            requires std::is_trivially_copyable_v<std::remove_cv_t<T> >
        static result create(std::span<T, Extent> data, gl_enum usage) noexcept {
            return create(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()), usage);
        }

        // try wrappers

        static vertex_buffer create_try(const void *data, gl_sizeiptr size, gl_enum usage) noexcept;

        template<class T, std::size_t Extent>
            requires std::is_trivially_copyable_v<std::remove_cv_t<T> >
        static vertex_buffer create_try(std::span<T, Extent> data, gl_enum usage) noexcept {
            return create_try(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()), usage);
        }

        // api

        void bind() const noexcept;

        static void unbind() noexcept;

        void set_data(const void *data, gl_sizeiptr size) noexcept;

        template<typename T, std::size_t Extent>
            requires std::is_trivially_copyable_v<T>
        void set_data(std::span<T, Extent> data) noexcept {
            set_data(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()));
        }

        [[nodiscard]] gl_uint id() const noexcept { return m_id; }
        [[nodiscard]] gl_sizeiptr size() const noexcept { return m_size; }
        [[nodiscard]] gl_enum usage() const noexcept { return m_usage; }

        inline constexpr static const char *err_to_str(error e) noexcept {
            switch (e) {
                case error::invalid_params: return "invalid params";
                case error::gl_gen_buffers_failed: return "glGenBuffers() failed";
                case error::gl_alloc_failed: return "glBufferData() failed to allocate";
                default: return "unknown vertex_buffer_error";
            }
        }

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
