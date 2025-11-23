#include "sgl.h"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

static auto VERTEX_SHADER_SOURCE = R"(
    #version 330 core

    layout (location = 0) in vec3 a_pos;
    layout (location = 1) in vec3 a_color;

    out vec3 color;

    void main() {
        gl_Position = vec4(a_pos, 1.0);
        color = a_color;
    }
)";

static auto FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core

    out vec4 frag_color;

    in vec3 color;

    void main() {
        frag_color = vec4(color, 1.0);
    }
)";

int main() {
    auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    auto shader = sgl::shader::create_from_source_or_panic(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);

    sgl::render::set_clear_color(sgl::colors::WHITE);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
