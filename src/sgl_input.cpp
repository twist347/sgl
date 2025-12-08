#include "internal/sgl_input.h"

#include <cstring>

#include "GLFW/glfw3.h"

namespace sgl::detail::input {
    inline constexpr int max_keys = static_cast<int>(key::count);
    inline constexpr int max_buttons = static_cast<int>(mouse_button::count);

    struct state {
        bool key_down[max_keys]{};
        bool key_down_prev[max_keys]{};

        bool mouse_down[max_buttons]{};
        bool mouse_down_prev[max_buttons]{};

        double mouse_x = 0.0;
        double mouse_y = 0.0;

        double mouse_dx = 0.0;
        double mouse_dy = 0.0;

        double scroll_dx = 0.0;
        double scroll_dy = 0.0;
    };

    static state g_state{};

    static key from_glfw_key(int glfw_key) noexcept {
        switch (glfw_key) {
            case GLFW_KEY_SPACE: return key::space;
            case GLFW_KEY_APOSTROPHE: return key::apostrophe;
            case GLFW_KEY_COMMA: return key::comma;
            case GLFW_KEY_MINUS: return key::minus;
            case GLFW_KEY_PERIOD: return key::period;
            case GLFW_KEY_SLASH: return key::slash;

            case GLFW_KEY_0: return key::num0;
            case GLFW_KEY_1: return key::num1;
            case GLFW_KEY_2: return key::num2;
            case GLFW_KEY_3: return key::num3;
            case GLFW_KEY_4: return key::num4;
            case GLFW_KEY_5: return key::num5;
            case GLFW_KEY_6: return key::num6;
            case GLFW_KEY_7: return key::num7;
            case GLFW_KEY_8: return key::num8;
            case GLFW_KEY_9: return key::num9;

            case GLFW_KEY_A: return key::a;
            case GLFW_KEY_B: return key::b;
            case GLFW_KEY_C: return key::c;
            case GLFW_KEY_D: return key::d;
            case GLFW_KEY_E: return key::e;
            case GLFW_KEY_F: return key::f;
            case GLFW_KEY_G: return key::g;
            case GLFW_KEY_H: return key::h;
            case GLFW_KEY_I: return key::i;
            case GLFW_KEY_J: return key::j;
            case GLFW_KEY_K: return key::k;
            case GLFW_KEY_L: return key::l;
            case GLFW_KEY_M: return key::m;
            case GLFW_KEY_N: return key::n;
            case GLFW_KEY_O: return key::o;
            case GLFW_KEY_P: return key::p;
            case GLFW_KEY_Q: return key::q;
            case GLFW_KEY_R: return key::r;
            case GLFW_KEY_S: return key::s;
            case GLFW_KEY_T: return key::t;
            case GLFW_KEY_U: return key::u;
            case GLFW_KEY_V: return key::v;
            case GLFW_KEY_W: return key::w;
            case GLFW_KEY_X: return key::x;
            case GLFW_KEY_Y: return key::y;
            case GLFW_KEY_Z: return key::z;

            case GLFW_KEY_ESCAPE: return key::escape;
            case GLFW_KEY_ENTER: return key::enter;
            case GLFW_KEY_TAB: return key::tab;
            case GLFW_KEY_BACKSPACE: return key::backspace;
            case GLFW_KEY_INSERT: return key::insert;
            case GLFW_KEY_DELETE: return key::del;

            case GLFW_KEY_RIGHT: return key::right;
            case GLFW_KEY_LEFT: return key::left;
            case GLFW_KEY_DOWN: return key::down;
            case GLFW_KEY_UP: return key::up;

            default: return key::unknown;
        }
    }

    void on_key(int glfw_key, int action) noexcept {
        const key k = from_glfw_key(glfw_key);
        if (k == key::unknown) {
            return;
        }

        const int idx = static_cast<int>(k);

        if (action == GLFW_PRESS) {
            g_state.key_down[idx] = true;
        } else if (action == GLFW_RELEASE) {
            g_state.key_down[idx] = false;
        }
    }

    static mouse_button from_glfw_mouse_button(int button) noexcept {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT: return mouse_button::left;
            case GLFW_MOUSE_BUTTON_RIGHT: return mouse_button::right;
            case GLFW_MOUSE_BUTTON_MIDDLE: return mouse_button::middle;
            default: return mouse_button::unknown;
        }
    }

    void on_mouse_button(int button, int action) noexcept {
        const mouse_button b = from_glfw_mouse_button(button);
        if (b == mouse_button::unknown) {
            return;
        }

        const int idx = static_cast<int>(b);

        if (action == GLFW_PRESS) {
            g_state.mouse_down[idx] = true;
        } else if (action == GLFW_RELEASE) {
            g_state.mouse_down[idx] = false;
        }
    }

    void on_cursor_pos(double x, double y) noexcept {
        const double dx = x - g_state.mouse_x;
        const double dy = y - g_state.mouse_y;

        g_state.mouse_x = x;
        g_state.mouse_y = y;

        g_state.mouse_dx += dx;
        g_state.mouse_dy += dy;
    }

    void on_scroll(double x_offset, double y_offset) noexcept {
        g_state.scroll_dx += x_offset;
        g_state.scroll_dy += y_offset;
    }


    void new_frame() noexcept {
        std::memcpy(g_state.key_down_prev, g_state.key_down, sizeof(g_state.key_down));
        std::memcpy(g_state.mouse_down_prev, g_state.mouse_down, sizeof(g_state.mouse_down));

        g_state.mouse_dx = 0.0;
        g_state.mouse_dy = 0.0;
        g_state.scroll_dx = 0.0;
        g_state.scroll_dy = 0.0;
    }

    static bool valid_key(key k) noexcept {
        const int idx = static_cast<int>(k);
        return idx >= 0 && idx < max_keys;
    }

    static bool is_down_impl(key k) noexcept {
        if (!valid_key(k)) {
            return false;
        }
        return g_state.key_down[static_cast<int>(k)];
    }

    static bool was_down_impl(key k) noexcept {
        if (!valid_key(k)) {
            return false;
        }
        return g_state.key_down_prev[static_cast<int>(k)];
    }

    static bool valid_mouse_button(mouse_button b) noexcept {
        const int idx = static_cast<int>(b);
        return idx >= 0 && idx < max_buttons;
    }

    static bool mouse_down_impl(mouse_button b) noexcept {
        if (!valid_mouse_button(b)) {
            return false;
        }
        return g_state.mouse_down[static_cast<int>(b)];
    }

    static bool mouse_was_down_impl(mouse_button b) noexcept {
        if (!valid_mouse_button(b)) {
            return false;
        }
        return g_state.mouse_down_prev[static_cast<int>(b)];
    }

    static void mouse_position_impl(double &x, double &y) noexcept {
        x = g_state.mouse_x;
        y = g_state.mouse_y;
    }

    static void mouse_delta_impl(double &dx, double &dy) noexcept {
        dx = g_state.mouse_dx;
        dy = g_state.mouse_dy;
    }

    static void scroll_delta_impl(double &dx, double &dy) noexcept {
        dx = g_state.scroll_dx;
        dy = g_state.scroll_dy;
    }
}

namespace sgl::input {
    bool is_key_down(key k) noexcept {
        return detail::input::is_down_impl(k);
    }

    bool is_key_up(key k) noexcept {
        return !detail::input::is_down_impl(k);
    }

    bool is_key_pressed(key k) noexcept {
        const bool now = detail::input::is_down_impl(k);
        const bool prev = detail::input::was_down_impl(k);
        return now && !prev;
    }

    bool is_key_released(key k) noexcept {
        const bool now = detail::input::is_down_impl(k);
        const bool prev = detail::input::was_down_impl(k);
        return !now && prev;
    }

    bool is_mouse_button_down(mouse_button b) noexcept {
        return detail::input::mouse_down_impl(b);
    }

    bool is_mouse_button_up(mouse_button b) noexcept {
        return !detail::input::mouse_down_impl(b);
    }

    bool is_mouse_button_pressed(mouse_button b) noexcept {
        const bool now = detail::input::mouse_down_impl(b);
        const bool prev = detail::input::mouse_was_down_impl(b);
        return now && !prev;
    }

    bool is_mouse_button_released(mouse_button b) noexcept {
        const bool now = detail::input::mouse_down_impl(b);
        const bool prev = detail::input::mouse_was_down_impl(b);
        return !now && prev;
    }

    void mouse_pos(double &x, double &y) noexcept {
        detail::input::mouse_position_impl(x, y);
    }

    void mouse_dt(double &dx, double &dy) noexcept {
        detail::input::mouse_delta_impl(dx, dy);
    }

    void scroll_dt(double &dx, double &dy) noexcept {
        detail::input::scroll_delta_impl(dx, dy);
    }
}
