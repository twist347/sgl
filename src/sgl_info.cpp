#include "internal/sgl_info.h"

#include "glad/glad.h"

#include "internal/sgl_type.h"
#include "internal/sgl_log.h"
#include "internal/sgl_util.h"
#include "internal/sgl_gl_info.h"

namespace sgl::detail {
    void print_info() noexcept {
        const auto info_res = get_gl_info();
        if (!info_res) {
            return;
        }

        const auto &[
            version,
            version_str,
            shading_language_str,
            vendor,
            renderer,
            profile_mask,
            num_extensions
        ] = *info_res;

        auto profile_str = "Unknown";
        if (profile_mask & GL_CONTEXT_CORE_PROFILE_BIT) {
            profile_str = "Core";
        } else if (profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
            profile_str = "Compatibility";
        }

        SGL_LOG_INFO("Version:    %d.%d (%s)",
                     version.major, version.minor,
                     version_str.empty() ? "?" : version_str.c_str());
        SGL_LOG_INFO("GLSL:       %s", shading_language_str.empty() ? "?" : shading_language_str.c_str());
        SGL_LOG_INFO("Vendor:     %s", vendor.empty() ? "?" : vendor.c_str());
        SGL_LOG_INFO("Renderer:   %s", renderer.empty() ? "?" : renderer.c_str());
        SGL_LOG_INFO("Profile:    %s", profile_str);
        SGL_LOG_INFO("Extensions: %d", static_cast<int>(num_extensions));
    }
}
