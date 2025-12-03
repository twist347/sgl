#include "internal/sgl_render.h"

#include "glad/glad.h"

namespace sgl::render {
    namespace detail {
        inline constexpr float inv_255 = 1.f / 255.f;
    }

    void set_clear_color(float r, float g, float b, float a) noexcept {
        glClearColor(r, g, b, a);
    }

    void set_clear_color(const color &c) noexcept {
        glClearColor(
            static_cast<float>(c.r) * detail::inv_255,
            static_cast<float>(c.g) * detail::inv_255,
            static_cast<float>(c.b) * detail::inv_255,
            static_cast<float>(c.a) * detail::inv_255
        );
    }

    void clear_color_buffer() noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void clear_color_depth_buffer() noexcept {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
