#pragma once

#include <string>
#include <utility>

#include "sgl_expected.h"

// forward decl
struct GLFWwindow;
struct GLFWmonitor;

namespace sgl {
    enum class window_error {
        invalid_params = 0,
        glfw_init_failed,
        glfw_create_window_failed,
        glad_load_failed,
        count
    };

    struct window_params {
        int width, height;
        const char *title;
        int opengl_min_ver_major = 3, opengl_min_ver_minor = 3;
        int fps = 60;
        bool show_fps = true;
        bool vsync = true;
        bool cursor_enabled = true;
        bool fullscreen = false;
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

        static result create(const window_params &params) noexcept;

        // try wrappers

        static window create_try(const window_params &params) noexcept;

        // api

        void make_current() const noexcept;

        void set_vsync(bool enabled) const noexcept;

        void set_cursor_enabled(bool enabled) const noexcept;

        void set_show_fps(bool enabled) const noexcept;

        [[nodiscard]] bool should_close() const noexcept;

        void swap_buffers() const noexcept;

        [[nodiscard]] int width() const noexcept;

        [[nodiscard]] int height() const noexcept;

        [[nodiscard]] std::pair<int, int> sizes() const noexcept;

        GLFWwindow *handle() const noexcept;

        static void poll_events() noexcept;

        constexpr static const char *err_to_str(error err) noexcept;

    private:
        static void init_viewport(GLFWwindow *handle) noexcept;

        static void framebuffer_size_callback(GLFWwindow *handle, int width, int height) noexcept;

        static void key_callback(GLFWwindow *handle, int key, int scancode, int action, int mods) noexcept;

        static void mouse_button_callback(GLFWwindow *handle, int button, int action, int mods) noexcept;

        static void cursor_pos_callback(GLFWwindow *handle, double x_pos, double y_pos) noexcept;

        static void scroll_callback(GLFWwindow *handle, double x_offset, double y_offset) noexcept;

        static int s_window_count;

    private:
        struct fps_state {
            bool enabled = false;
            double last_time = 0.0;
            int frames = 0;
            double interval_sec = 1.0;
            double cur_fps = 0.0;
            std::string base_title;
        };

        mutable fps_state m_fps_state;

        void count_fps() const noexcept;

    private:
        explicit window(GLFWwindow *handle) noexcept : m_window{handle} {
        }

        static result create_impl(
            int width, int height, const char *title,
            GLFWmonitor *monitor,
            const window_params *params
        ) noexcept;

        void destroy_window() noexcept;

        GLFWwindow *m_window = nullptr;
    };
}
