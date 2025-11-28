#include "internal/sgl_texture.h"

#include <utility>

#include "glad/glad.h"

#include "internal/sgl_log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sgl {
    // ctors and assignments

    texture_2d::texture_2d(texture_2d &&other) noexcept
        : m_id(std::exchange(other.m_id, 0))
          , m_width(std::exchange(other.m_width, 0))
          , m_height(std::exchange(other.m_height, 0))
          , m_internal_format(std::exchange(other.m_internal_format, 0))
          , m_format(std::exchange(other.m_format, 0)) {
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
        if (!path) {
            SGL_LOG_ERROR("texture_2d::create_from_file: path is null");
            return unexpected(error::INVALID_PARAMS);
        }

        int width = 0;
        int height = 0;
        int nr_channels = 0;

        stbi_set_flip_vertically_on_load(1);

        stbi_uc *data = stbi_load(path, &width, &height, &nr_channels, 0);
        if (!data) {
            SGL_LOG_ERROR("texture_2d::create_from_file: failed to load image: %s", path);
            return unexpected(error::STBI_LOAD_FAILED);
        }

        gl_uint id = 0;
        glGenTextures(1, &id);
        if (id == 0) {
            SGL_LOG_ERROR("texture_2d::create_from_file: glGenTextures() returned 0");
            stbi_image_free(data);
            return unexpected(error::GL_GEN_FAILED);
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

        const auto internal_format = static_cast<GLint>(format);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
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

    const char *texture_2d::err_to_str(error e) noexcept {
        switch (e) {
            case error::INVALID_PARAMS: return "invalid params";
            case error::STBI_LOAD_FAILED: return "stbi_load() failed";
            case error::GL_GEN_FAILED: return "glGenTextures() failed";
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
