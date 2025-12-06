#include "internal/sgl_gl_info.h"

#include <cstring>

#include "glad/glad.h"

#include "internal/sgl_backend.h"
#include "internal/sgl_log.h"
#include "internal/sgl_type.h"

namespace sgl {
    gl_info_result get_gl_info() noexcept {
        gl_info info;

        if (!detail::backend::ensure_glfw() || !detail::backend::ensure_glad()) {
            SGL_LOG_ERROR("get_gl_info(): backend not initialized");
            return unexpected{gl_info_error::backend_not_initialized};
        }

        const auto *vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        const auto *renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        const auto *version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        const auto *glsl = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        if (vendor) {
            info.vendor = vendor;
        }
        if (renderer) {
            info.renderer = renderer;
        }
        if (version) {
            info.version_str = version;
        }
        if (glsl) {
            info.shading_language_str = glsl;
        }

        gl_int major = 0;
        gl_int minor = 0;
        gl_int profile = 0;
        gl_int num_ext = 0;

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext);

        info.version.major = major;
        info.version.minor = minor;
        info.profile_mask = profile;
        info.num_extensions = num_ext;

        return info;
    }

    bool has_extension(const char *name) noexcept {
        if (!name || *name == '\0') {
            return false;
        }

        if (!detail::backend::ensure_glfw() || !detail::backend::ensure_glad()) {
            SGL_LOG_ERROR("has_extension(): backend not initialized");
            return false;
        }

        gl_int num_ext = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext);

        if (num_ext > 0) {
            const std::size_t len = std::strlen(name);
            for (gl_int i = 0; i < num_ext; ++i) {
                const auto *ext = reinterpret_cast<const char *>(
                    glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i))
                );
                if (!ext) {
                    continue;
                }
                if (std::strlen(ext) == len && std::memcmp(ext, name, len) == 0) {
                    return true;
                }
            }
            return false;
        }
        return false;
    }
}
