#include "internal/sgl_window.h"

#include <utility>
#include <cstdio>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "internal/sgl_log.h"
#include "internal/sgl_util.h"
#include "internal/sgl_info.h"
#include "internal/sgl_backend.h"
#include "internal/sgl_time.h"

namespace sgl {
    int window::s_window_count = 0;

    // ctors

    window::window(window &&other) noexcept : m_fps_state{std::move(other.m_fps_state)},
                                              m_window{std::exchange(other.m_window, nullptr)} {
    }

    window &window::operator=(window &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        destroy_window();

        m_fps_state = std::move(other.m_fps_state);
        m_window = std::exchange(other.m_window, nullptr);

        return *this;
    }

    window::~window() {
        destroy_window();
    }

    // fabrics

    window::result window::create(int width, int height, const char *title, bool show_fps) noexcept {
        if (width <= 0 || height <= 0 || !title) {
            return unexpected{error::invalid_params};
        }

        if (!detail::backend::ensure_glfw()) {
            return unexpected{error::glfw_init_failed};
        }

        // reset
        glfwDefaultWindowHints();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        GLFWwindow *handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!handle) {
            return unexpected{error::glfw_create_window_failed};
        }

        glfwMakeContextCurrent(handle);

        if (!detail::backend::ensure_glad()) {
            glfwDestroyWindow(handle);
            return unexpected{error::glad_load_failed};
        }

        ++s_window_count;

        init_viewport(handle);

        glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
        glfwSetKeyCallback(handle, key_callback);

        if (s_window_count == 1) {
            detail::print_info();
        }

        auto w = window{handle};

        if (show_fps) {
            w.m_fps_state.enabled = true;
            w.m_fps_state.last_time = get_time();
            w.m_fps_state.base_title = title;
        }

        return w;
    }

    // or panic wrappers

    window window::create_or_panic(int width, int height, const char *title, bool show_fps) noexcept {
        auto res = create(width, height, title, show_fps);
        if (!res) {
            SGL_LOG_FATAL("failed to create window: %s", err_to_str(res.error()));
        }
        return std::move(*res);
    }

    // api

    void window::make_current() const noexcept {
        glfwMakeContextCurrent(m_window);
    }

    void window::set_vsync(bool enabled) const noexcept {
        GLFWwindow *prev = glfwGetCurrentContext();
        if (prev != m_window) {
            glfwMakeContextCurrent(m_window);
        }

        glfwSwapInterval(enabled ? 1 : 0);

        if (prev != m_window) {
            glfwMakeContextCurrent(prev);
        }
    }

    bool window::should_close() const noexcept {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    void window::swap_buffers() const noexcept {
        glfwSwapBuffers(m_window);

        if (m_fps_state.enabled) {
            count_fps();
        }
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

    void window::poll_events() noexcept {
        glfwPollEvents();
    }

    constexpr const char *window::err_to_str(window_error err) noexcept {
        switch (err) {
            case window_error::invalid_params: return "invalid params";
            case window_error::glfw_init_failed: return "glfwInit() failed";
            case window_error::glfw_create_window_failed: return "glfwCreateWindow() failed";
            case window_error::glad_load_failed: return "gladLoadGLLoader() failed";
            default: return "unknown window_error";
        }
    }

    void window::framebuffer_size_callback(GLFWwindow *handle, int width, int height) noexcept {
        SGL_UNUSED(handle);

        glViewport(0, 0, width, height);
    }

    void window::key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) noexcept {
        SGL_UNUSED(scancode);
        SGL_UNUSED(mods);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(handle, GLFW_TRUE);
        }
    }

    void window::init_viewport(GLFWwindow *handle) noexcept {
        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(handle, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);
    }

    void window::count_fps() const noexcept {
        const double now = get_time();
        ++m_fps_state.frames;

        if (const auto dt = now - m_fps_state.last_time; dt >= m_fps_state.interval_sec && dt > 0.0) {
            m_fps_state.cur_fps = static_cast<double>(m_fps_state.frames) / dt;
            m_fps_state.frames = 0;
            m_fps_state.last_time = now;

            char buf[128];
            std::snprintf(
                buf, sizeof(buf), "%s [%.0f FPS]",
                m_fps_state.base_title.c_str(),
                m_fps_state.cur_fps
            );
            glfwSetWindowTitle(m_window, buf);
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
