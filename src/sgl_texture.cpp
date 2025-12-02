#include "internal/sgl_texture.h"

#include <utility>

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "internal/sgl_log.h"

namespace {
    constexpr sgl::gl_enum to_gl(sgl::texture_wrap wrap) noexcept {
        switch (wrap) {
            case sgl::texture_wrap::repeat: return GL_REPEAT;
            case sgl::texture_wrap::mirrored_repeat: return GL_MIRRORED_REPEAT;
            case sgl::texture_wrap::clamp_to_edge: return GL_CLAMP_TO_EDGE;
            case sgl::texture_wrap::clamp_to_border: return GL_CLAMP_TO_BORDER;
        }
        return GL_REPEAT;
    }

    constexpr sgl::gl_enum to_gl(sgl::texture_min_filter f) noexcept {
        switch (f) {
            case sgl::texture_min_filter::nearest: return GL_NEAREST;
            case sgl::texture_min_filter::linear: return GL_LINEAR;
            case sgl::texture_min_filter::nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
            case sgl::texture_min_filter::linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
            case sgl::texture_min_filter::nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
            case sgl::texture_min_filter::linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
        }
        return GL_LINEAR;
    }

    constexpr sgl::gl_enum to_gl(sgl::texture_mag_filter f) noexcept {
        switch (f) {
            case sgl::texture_mag_filter::nearest: return GL_NEAREST;
            case sgl::texture_mag_filter::linear: return GL_LINEAR;
        }
        return GL_LINEAR;
    }
}

namespace sgl {
    // ctors and assignments

    texture_2d::texture_2d(texture_2d &&other) noexcept
        : m_id{std::exchange(other.m_id, 0)},
          m_width{std::exchange(other.m_width, 0)},
          m_height{std::exchange(other.m_height, 0)},
          m_internal_format{std::exchange(other.m_internal_format, 0)},
          m_format{std::exchange(other.m_format, 0)} {
    }

    texture_2d &texture_2d::operator=(texture_2d &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy();

        m_id = std::exchange(other.m_id, 0);
        m_width = std::exchange(other.m_width, 0);
        m_height = std::exchange(other.m_height, 0);
        m_internal_format = std::exchange(other.m_internal_format, 0);
        m_format = std::exchange(other.m_format, 0);

        return *this;
    }

    texture_2d::~texture_2d() {
        destroy();
    }

    // fabrics

    texture_2d::result texture_2d::create_from_file(const char *path) noexcept {
        const texture_2d_params params{};
        return create_from_file(path, params);
    }

    texture_2d::result texture_2d::create_from_file(const char *path, const texture_2d_params &params) noexcept {
        if (!path) {
            SGL_LOG_ERROR("texture_2d::create_from_file: path is null");
            return unexpected(error::invalid_params);
        }

        int width = 0, height = 0, nr_channels = 0;

        stbi_set_flip_vertically_on_load(params.flip_vertically_on_load ? 1 : 0);

        stbi_uc *data = stbi_load(path, &width, &height, &nr_channels, 0);
        if (!data) {
            SGL_LOG_ERROR("texture_2d::create_from_file: failed to load image: %s", path);
            return unexpected(error::stbi_load_failed);
        }

        gl_uint id = 0;
        glGenTextures(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("texture_2d::create_from_file: glGenTextures() returned 0");
            stbi_image_free(data);
            return unexpected(error::gl_gen_failed);
        }

        GLint prev_tex = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

        GLint prev_alignment = 0;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prev_alignment);

        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, to_gl(params.wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, to_gl(params.wrap_t));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, to_gl(params.min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, to_gl(params.mag_filter));

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        GLenum format;
        if (nr_channels == 1) {
            format = GL_RED;
        } else if (nr_channels == 3) {
            format = GL_RGB;
        } else if (nr_channels == 4) {
            format = GL_RGBA;
        } else {
            SGL_LOG_WARN("texture_2d::create_from_file: unsupported channel count %d, assuming RGB", nr_channels);
            format = GL_RGB;
        }

        auto internal_format = static_cast<GLint>(format);
        if (params.srgb) {
            if (format == GL_RGB) {
                internal_format = GL_SRGB;
            }
            if (format == GL_RGBA) {
                internal_format = GL_SRGB_ALPHA;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        if (params.generate_mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        // restore
        glPixelStorei(GL_UNPACK_ALIGNMENT, prev_alignment);
        glBindTexture(GL_TEXTURE_2D, prev_tex);

        stbi_image_free(data);

        return texture_2d{id, width, height, static_cast<gl_enum>(internal_format), format};
    }

    // or panic wrappers

    texture_2d texture_2d::create_from_file_or_panic(const char *path) noexcept {
        auto res = create_from_file(path);
        if (!res) {
            SGL_LOG_FATAL("failed to create texture_2d from '%s': %s", path, err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void texture_2d::bind(gl_uint unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void texture_2d::unbind(gl_uint unit) noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    constexpr const char *texture_2d::err_to_str(error e) noexcept {
        switch (e) {
            case error::invalid_params: return "invalid params";
            case error::stbi_load_failed: return "stbi_load() failed";
            case error::gl_gen_failed: return "glGenTextures() failed";
            default: return "unknown texture_error";
        }
    }

    // internal

    void texture_2d::destroy() noexcept {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
            m_width = 0;
            m_height = 0;
            m_internal_format = 0;
            m_format = 0;
        }
    }
}
