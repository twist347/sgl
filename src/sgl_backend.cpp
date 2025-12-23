#include "internal/sgl_backend.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "internal/sgl_log.h"

namespace sgl::detail {
    static bool s_glfw_initialized = false;
    static bool s_glad_initialized = false;

    static void glfw_error_callback(int code, const char* desc);

    bool ensure_glfw() noexcept {
        if (s_glfw_initialized) {
            return true;
        }
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) {
            SGL_LOG_ERROR("ensure_glfw(): glfwInit() failed");
            return false;
        }
        s_glfw_initialized = true;
        return true;
    }

    bool ensure_glad() noexcept {
        if (s_glad_initialized) {
            return true;
        }
        if (glfwGetCurrentContext() == nullptr) {
            SGL_LOG_ERROR("ensure_glad(): no current context. Create window and make it current first.");
            return false;
        }
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            SGL_LOG_ERROR("ensure_glad(): gladLoadGLLoader failed.");
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

    static void glfw_error_callback(int code, const char* desc) {
        SGL_LOG_ERROR("GLFW error %d: %s", code, desc ? desc : "?");
    }
}
