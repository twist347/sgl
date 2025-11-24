#pragma once

#include <string>

#include "sgl_expected.h"

// forward decl
struct GLFWwindow;

namespace sgl {
    enum class window_error {
        INVALID_PARAMS = 0,
        GLFW_INIT_FAILED,
        GLFW_CREATE_WINDOW_FAILED,
        GLAD_LOAD_FAILED,
        COUNT
    };

    class window {
    public:
        using error = window_error;
        using result = expected<window, error>;

        // ctors and assignments

        window(const window &) = delete;

        window &operator=(const window &) = delete;

        window(window &&other) noexcept;

        window &operator=(window &&other) noexcept;

        ~window();

        // fabrics

        static result create(int width, int height, const char *title) noexcept;

        static result create(int width, int height, const std::string &title) noexcept {
            return create(width, height, title.c_str());
        }

        // or panic wrappers

        static window create_or_panic(int width, int height, const char *title) noexcept;

        static window create_or_panic(int width, int height, const std::string &title) noexcept {
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

        static const char *err_to_str(error err) noexcept;

    private:
        explicit window(GLFWwindow *handle) noexcept : m_window(handle) {
        }

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static int s_window_count;

    private:
        void destroy_window() noexcept;

        GLFWwindow *m_window = nullptr;
    };
}
