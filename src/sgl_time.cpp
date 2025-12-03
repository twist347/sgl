#include "internal/sgl_time.h"

#include "GLFW/glfw3.h"

namespace sgl {
    double get_time() noexcept {
        return glfwGetTime();
    }

    float get_time_f() noexcept {
        return static_cast<float>(glfwGetTime());
    }
}
