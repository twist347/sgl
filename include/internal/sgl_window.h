#pragma once

#include <string>

#include "sgl_expected.h"

// forward decl
struct GLFWwindow;

namespace sgl {
    enum class window_error {
        invalid_params = 0,
        glfw_init_failed,
        glfw_create_window_failed,
        glad_load_failed,
        count
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

        void make_current() const noexcept;

        void set_vsync(bool enabled) const noexcept;

        [[nodiscard]] bool should_close() const noexcept;

        void swap_buffers() const noexcept;

        [[nodiscard]] int width() const noexcept;

        [[nodiscard]] int height() const noexcept;

        static void poll_events() noexcept;

        constexpr static const char *err_to_str(error err) noexcept;

    private:
        explicit window(GLFWwindow *handle) noexcept : m_window{handle} {
        }

        static void framebuffer_size_callback(GLFWwindow *handle, int width, int height) noexcept;

        static void key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) noexcept;

        static void init_viewport(GLFWwindow *handle) noexcept;

        static int s_window_count;

    private:
        void destroy_window() noexcept;

        GLFWwindow *m_window = nullptr;
    };
}
