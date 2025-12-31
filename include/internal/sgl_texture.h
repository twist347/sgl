#pragma once

#include <string>

#include "sgl_expected.h"
#include "sgl_type.h"

namespace sgl {
    enum class texture_error {
        invalid_params = 0,
        stbi_load_failed,
        gl_gen_failed,
        count
    };

    enum class texture_wrap {
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border,
    };

    enum class texture_min_filter {
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    enum class texture_mag_filter {
        nearest,
        linear,
    };

    struct texture_2d_params {
        texture_wrap wrap_s = texture_wrap::repeat;
        texture_wrap wrap_t = texture_wrap::repeat;

        texture_min_filter min_filter = texture_min_filter::linear_mipmap_linear;
        texture_mag_filter mag_filter = texture_mag_filter::linear;

        bool generate_mipmaps = true;
        bool flip_vertically_on_load = true;

        bool srgb = false;
    };

    class texture_2d {
    public:
        using error = texture_error;
        using result = expected<texture_2d, error>;

        // ctors and assignments

        texture_2d(const texture_2d &) = delete;

        texture_2d &operator=(const texture_2d &) = delete;

        texture_2d(texture_2d &&other) noexcept;

        texture_2d &operator=(texture_2d &&other) noexcept;

        ~texture_2d();

        // fabrics

        static result create_from_file(const char *path) noexcept;

        static result create_from_file(const std::string &path) noexcept {
            return create_from_file(path.c_str());
        }

        static result create_from_file(const char *path, const texture_2d_params &params) noexcept;

        static result create_from_file(const std::string &path, const texture_2d_params &params) noexcept {
            return create_from_file(path.c_str(), params);
        }

        // try wrappers

        static texture_2d create_from_file_try(const char *path) noexcept;

        static texture_2d create_from_file_try(const std::string &path) noexcept {
            return create_from_file_try(path.c_str());
        }

        static texture_2d create_from_file_try(const char *path, const texture_2d_params &params) noexcept;

        static texture_2d create_from_file_try(const std::string &path, const texture_2d_params &params) noexcept {
            return create_from_file_try(path.c_str(), params);
        }

        // api

        void bind(gl_uint unit) const noexcept;

        static void unbind(gl_uint unit) noexcept;

        [[nodiscard]] gl_uint id() const noexcept { return m_id; }

        [[nodiscard]] gl_int width() const noexcept { return m_width; }
        [[nodiscard]] gl_int height() const noexcept { return m_height; }

        [[nodiscard]] gl_enum internal_format() const noexcept { return m_internal_format; }
        [[nodiscard]] gl_enum format() const noexcept { return m_format; }

        constexpr static const char *err_to_str(error e) noexcept;

    private:
        explicit texture_2d(gl_uint id, gl_int w, gl_int h, gl_enum internal_format, gl_enum format) noexcept
            : m_id{id}, m_width{w}, m_height{h}, m_internal_format{internal_format}, m_format{format} {
        }

        void destroy() noexcept;

        gl_uint m_id = 0;
        gl_int m_width = 0;
        gl_int m_height = 0;
        gl_enum m_internal_format = 0;
        gl_enum m_format = 0;
    };
}
