#pragma once

#include <string>

#include "sgl_expected_facade.h"

// forward decl
struct GLFWwindow;

namespace sgl {
    enum class window_error_e {
        INVALID_PARAMS = 0,
        GLFW_INIT_FAILED,
        GLFW_CREATE_WINDOW_FAILED,
        GLAD_LOAD_FAILED,
        COUNT
    };

    class window_t {
    public:
        using error_t = window_error_e;
        using expected_t = expected<window_t, error_t>;

        // ctors

        window_t(const window_t &) = delete;

        window_t &operator=(const window_t &) = delete;

        window_t(window_t &&other) noexcept;

        window_t &operator=(window_t &&other) noexcept;

        ~window_t();

        // fabrics

        static expected_t create(int width, int height, const char *title) noexcept;

        static expected_t create(int width, int height, const std::string &title) noexcept {
            return create(width, height, title.c_str());
        }

        // or panic wrappers

        static window_t create_or_panic(int width, int height, const char *title) noexcept;

        static window_t create_or_panic(int width, int height, const std::string &title) noexcept {
            return create_or_panic(width, height, title.c_str());
        }

        // api

        void make_current();

        void set_vsync(bool enabled);

        [[nodiscard]] bool should_close() const;

        void swap_buffers();

        [[nodiscard]] int width() const noexcept;

        [[nodiscard]] int height() const noexcept;

        static void poll_events();

        static const char *err_to_str(window_error_e err) noexcept;

    private:
        explicit window_t(GLFWwindow *handle) noexcept : m_window(handle) {
        }

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static bool s_glfw_initialized;
        static bool s_glad_initialized;
        static int s_window_count;

    private:
        GLFWwindow *m_window = nullptr;
    };
}
