#include "internal/sgl_window.h"

#include <utility>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "internal/sgl_log.h"
#include "internal/sgl_util.h"
#include "internal/sgl_info.h"
#include "internal/sgl_backend.h"

namespace sgl {
    int window::s_window_count = 0;

    // ctors

    window::window(window &&other) noexcept : m_window(std::exchange(other.m_window, nullptr)) {
    }

    window &window::operator=(window &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy_window();

        m_window = std::exchange(other.m_window, nullptr);

        return *this;
    }

    window::~window() {
        destroy_window();
    }

    // fabrics

    window::result window::create(int width, int height, const char *title) noexcept {
        if (width <= 0 || height <= 0 || !title) {
            return unexpected{error::INVALID_PARAMS};
        }

        if (!detail::backend::ensure_glfw()) {
            return unexpected{error::GLFW_INIT_FAILED};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!handle) {
            return unexpected{error::GLFW_CREATE_WINDOW_FAILED};
        }

        glfwMakeContextCurrent(handle);

        if (!detail::backend::ensure_glad()) {
            glfwDestroyWindow(handle);
            return unexpected{error::GLAD_LOAD_FAILED};
        }

        ++s_window_count;

        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(handle, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);

        glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
        glfwSetKeyCallback(handle, key_callback);

        detail::print_info();

        return window{handle};
    }

    // or panic wrappers

    window window::create_or_panic(int width, int height, const char *title) noexcept {
        auto res = create(width, height, title);
        if (!res) {
            SGL_LOG_FATAL("failed to create window: %s", err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void window::make_current() {
        glfwMakeContextCurrent(m_window);
    }

    void window::set_vsync(bool enabled) {
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(enabled ? 1 : 0);
    }

    bool window::should_close() const {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    void window::swap_buffers() {
        glfwSwapBuffers(m_window);
    }

    int window::width() const noexcept {
        int w = 0, h = 0;
        glfwGetWindowSize(m_window, &w, &h);
        return w;
    }

    int window::height() const noexcept {
        int w = 0, h = 0;
        glfwGetWindowSize(m_window, &w, &h);
        return h;
    }

    void window::poll_events() {
        glfwPollEvents();
    }

    const char *window::err_to_str(window_error err) noexcept {
        switch (err) {
            case window_error::INVALID_PARAMS: return "invalid params";
            case window_error::GLFW_INIT_FAILED: return "glfwInit() failed";
            case window_error::GLFW_CREATE_WINDOW_FAILED: return "glfwCreateWindow() failed";
            case window_error::GLAD_LOAD_FAILED: return "gladLoadGLLoader() failed";
            default: return "unknown window_error";
        }
    }

    void window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        SGL_UNUSED(window);

        glViewport(0, 0, width, height);
    }

    void window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        SGL_UNUSED(scancode);
        SGL_UNUSED(mods);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    void window::destroy_window() noexcept {
        if (!m_window) {
            return;
        }

        glfwDestroyWindow(m_window);
        m_window = nullptr;

        --s_window_count;
        if (s_window_count == 0) {
            glfwTerminate();
            detail::backend::reset();
        }
    }
}
