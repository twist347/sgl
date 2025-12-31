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

        log_info("Version:    {}.{} ({})",
                     version.major, version.minor,
                     version_str.empty() ? "?" : version_str.c_str());
        log_info("GLSL:       {}", shading_language_str.empty() ? "?" : shading_language_str.c_str());
        log_info("Vendor:     {}", vendor.empty() ? "?" : vendor.c_str());
        log_info("Renderer:   {}", renderer.empty() ? "?" : renderer.c_str());
        log_info("Profile:    {}", profile_str);
        log_info("Extensions: {}", static_cast<int>(num_extensions));
    }
}
