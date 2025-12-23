#pragma once

#include <string>

#include "sgl_type.h"
#include "sgl_expected.h"

namespace sgl {
    struct gl_version {
        gl_int major{0};
        gl_int minor{0};
    };

    struct gl_info {
        gl_version version;                 // GL_MAJOR_VERSION / GL_MINOR_VERSION

        std::string version_str;           // glGetString(GL_VERSION)
        std::string shading_language_str;  // glGetString(GL_SHADING_LANGUAGE_VERSION)
        std::string vendor;                // glGetString(GL_VENDOR)
        std::string renderer;              // glGetString(GL_RENDERER)

        gl_int profile_mask{0};           // GL_CONTEXT_PROFILE_MASK
        gl_int num_extensions{0};         // GL_NUM_EXTENSIONS (если доступно)
    };

    enum class gl_info_error {
        backend_not_initialized = 0,
        no_current_context,

        count
    };

    using gl_info_result = expected<gl_info, gl_info_error>;

    gl_info_result get_gl_info() noexcept;

    bool has_extension(const char *name) noexcept;
}
