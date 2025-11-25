#include "internal/sgl_backend.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace sgl::detail::backend {
    static bool s_glfw_initialized = false;
    static bool s_glad_initialized = false;

    bool ensure_glfw() noexcept {
        if (s_glfw_initialized) {
            return true;
        }
        if (!glfwInit()) {
            return false;
        }
        s_glfw_initialized = true;
        return true;
    }

    bool ensure_glad() noexcept {
        if (s_glad_initialized) {
            return true;
        }
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            return false;
        }
        s_glad_initialized = true;
        return true;
    }

    bool is_glfw_initialized() noexcept {
        return s_glfw_initialized;
    }

    bool is_glad_initialized() noexcept {
        return s_glad_initialized;
    }

    void reset() noexcept {
        s_glfw_initialized = false;
        s_glad_initialized = false;
    }
}
