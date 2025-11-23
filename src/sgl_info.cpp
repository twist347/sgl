#include "internal/sgl_info.h"
#include "internal/sgl_type.h"
#include "internal/sgl_log.h"

#include "glad/glad.h"

namespace sgl::info {
    void print_info() noexcept {
        gl_int major = 0, minor = 0, profile = 0, n_ext = 0;
        auto vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        auto renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        auto version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        auto glsl = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
        glGetIntegerv(GL_NUM_EXTENSIONS, &n_ext);

        const char *profile_str =
                profile & GL_CONTEXT_CORE_PROFILE_BIT
                    ? "Core"
                    : profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
                          ? "Compatibility"
                          : "Unknown";


        SGL_LOG_INFO("Version:    %d.%d (%s)", major, minor, version ? version : "?");
        SGL_LOG_INFO("GLSL:       %s", glsl ? glsl : "?");
        SGL_LOG_INFO("Vendor:     %s", vendor ? vendor : "?");
        SGL_LOG_INFO("Vendor:     %s", vendor ? vendor : "?");
        SGL_LOG_INFO("Renderer:   %s", renderer ? renderer : "?");
        SGL_LOG_INFO("Profile:    %s", profile_str);
        SGL_LOG_INFO("Extensions: %d", n_ext);
    }
}
