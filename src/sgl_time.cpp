#include "internal/sgl_time.h"

#include <algorithm>

#include "GLFW/glfw3.h"

namespace sgl {
    namespace {
        double g_last_time = 0.0;
        double g_delta_time = 0.0;
        bool g_first_frame = true;
    }

    double get_time() noexcept {
        return glfwGetTime();
    }

    float get_time_f() noexcept {
        return static_cast<float>(glfwGetTime());
    }

    double get_dt() noexcept {
        return g_delta_time;
    }

    float get_dt_f() noexcept {
        return static_cast<float>(g_delta_time);
    }

    void new_frame_time() noexcept {
        const double t = glfwGetTime();

        if (g_first_frame) {
            g_delta_time = 0.0;
            g_first_frame = false;
        } else {
            const double raw_dt = t - g_last_time;
            g_delta_time = std::clamp(raw_dt, 0.0, 0.25);
        }

        g_last_time = t;
    }
}
