#include "internal/sgl_window.h"
#include "internal/sgl_util.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "internal/sgl_log.h"

namespace sgl {
    bool window_t::s_glfw_initialized = false;
    bool window_t::s_glad_initialized = false;
    int window_t::s_window_count = 0;

    // ctors

    window_t::window_t(window_t &&other) noexcept : m_window(other.m_window) {
        other.m_window = nullptr;
    }

    window_t &window_t::operator=(window_t &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        if (m_window) {
            glfwDestroyWindow(m_window);
            --s_window_count;
            if (s_window_count == 0 && s_glfw_initialized) {
                glfwTerminate();
                s_glfw_initialized = false;
                s_glad_initialized = false;
            }
        }

        m_window = other.m_window;
        other.m_window = nullptr;
        return *this;
    }

    window_t::~window_t() {
        if (!m_window) {
            return;
        }

        glfwDestroyWindow(m_window);
        m_window = nullptr;

        --s_window_count;
        if (s_window_count == 0 && s_glfw_initialized) {
            glfwTerminate();
            s_glfw_initialized = false;
            s_glad_initialized = false;
        }
    }

    // fabrics

    window_t::expected_t window_t::create(int width, int height, const char *title) noexcept {
        if (width <= 0 || height <= 0 || !title) {
            return unexpected(window_error_e::INVALID_PARAMS);
        }

        if (!s_glfw_initialized) {
            if (!glfwInit()) {
                return unexpected(window_error_e::GLFW_INIT_FAILED);
            }
            s_glfw_initialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!handle) {
            return unexpected(window_error_e::GLFW_CREATE_WINDOW_FAILED);
        }

        glfwMakeContextCurrent(handle);

        if (!s_glad_initialized) {
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                glfwDestroyWindow(handle);
                return sgl::unexpected(window_error_e::GLAD_LOAD_FAILED);
            }
            s_glad_initialized = true;
        }

        ++s_window_count;

        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(handle, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);

        glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
        glfwSetKeyCallback(handle, key_callback);

        return window_t{handle};
    }

    // or panic wrappers

    window_t window_t::create_or_panic(int width, int height, const char *title) noexcept {
        auto window_exp = create(width, height, title);
        if (!window_exp) {
            SGL_LOG_FATAL("failed to create window: %s", err_to_str(window_exp.error()));
        }
        return *std::move(window_exp);
    }

    // api

    void window_t::make_current() {
        glfwMakeContextCurrent(m_window);
    }

    void window_t::set_vsync(bool enabled) {
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(enabled ? 1 : 0);
    }

    bool window_t::should_close() const {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    void window_t::swap_buffers() {
        glfwSwapBuffers(m_window);
    }

    int window_t::width() const noexcept {
        int w = 0, h = 0;
        glfwGetWindowSize(m_window, &w, &h);
        return w;
    }

    int window_t::height() const noexcept {
        int w = 0, h = 0;
        glfwGetWindowSize(m_window, &w, &h);
        return h;
    }

    void window_t::poll_events() {
        glfwPollEvents();
    }

    const char *window_t::err_to_str(window_error_e err) noexcept {
        switch (err) {
            case window_error_e::INVALID_PARAMS: return "invalid params";
            case window_error_e::GLFW_INIT_FAILED: return "glfwInit() failed";
            case window_error_e::GLFW_CREATE_WINDOW_FAILED: return "glfwCreateWindow() failed";
            case window_error_e::GLAD_LOAD_FAILED: return "gladLoadGLLoader() failed";
            default: return "unknown window_error";
        }
    }

    void window_t::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        SGL_UNUSED(window);

        glViewport(0, 0, width, height);
    }

    void window_t::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        SGL_UNUSED(scancode);
        SGL_UNUSED(mods);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}
