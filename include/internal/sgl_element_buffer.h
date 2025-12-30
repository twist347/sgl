#pragma once

#include <type_traits>
#include <span>

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

        template<typename Idx, std::size_t Extent>
            requires std::is_integral_v<Idx>
        static result create(std::span<Idx, Extent> data, gl_enum usage) noexcept {
            constexpr idx_type t = idx_type_for<Idx>();
            static_assert(t != static_cast<idx_type>(0), "element_buffer::create(span): unsupported index type");
            return create(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()), static_cast<gl_enum>(t), usage);
        }

        // try wrappers

        static element_buffer create_try(
            const void *data, gl_sizeiptr size, gl_enum index_type, gl_enum usage
        ) noexcept;

        template<typename Idx, std::size_t Extent>
            requires std::is_integral_v<Idx>
        static element_buffer create_try(std::span<Idx, Extent> data, gl_enum usage) noexcept {
            constexpr idx_type t = idx_type_for<Idx>();
            static_assert(t != static_cast<idx_type>(0), "element_buffer::create_try(span): unsupported index type");
            return create_try(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()), static_cast<gl_enum>(t), usage);
        }

        // api

        void bind() const noexcept;

        static void unbind_current_vao() noexcept;

        void set_data(const void *data, gl_sizeiptr size) noexcept;

        template<typename Idx, std::size_t Extent>
            requires std::is_integral_v<Idx>
        void set_data(std::span<Idx, Extent> data) noexcept {
            constexpr idx_type t = idx_type_for<Idx>();
            static_assert(t != static_cast<idx_type>(0), "element_buffer::set_data(span): unsupported index type");
            assert(m_type == static_cast<gl_enum>(t) && "element_buffer::set_data(span): index type mismatch");
            set_data(data.data(), static_cast<gl_sizeiptr>(data.size_bytes()));
        }

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

        template<typename Idx>
            requires std::is_integral_v<Idx>
        static consteval idx_type idx_type_for() noexcept {
            using I = std::remove_cv_t<Idx>;
            if constexpr (std::is_same_v<Idx, bool>) return static_cast<idx_type>(0);
            if constexpr (std::is_same_v<I, std::uint8_t> || std::is_same_v<I, gl_ubyte>) return idx_type::u8;
            if constexpr (std::is_same_v<I, std::uint16_t> || std::is_same_v<I, gl_ushort>) return idx_type::u16;
            if constexpr (std::is_same_v<I, std::uint32_t> || std::is_same_v<I, gl_uint>) return idx_type::u32;
            else return static_cast<idx_type>(0);
        }

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
