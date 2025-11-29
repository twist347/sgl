#include "sgl.h"

#include <array>

#include "glad/glad.h"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";

struct vertex {
    sgl::gl_float pos[3];
    sgl::color color;
};

int main() {
    auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    constexpr std::array<vertex, 2> vertices = {
        {
            {{-0.8f, 0.f, 0.f}, sgl::colors::RED},
            {{0.8f, 0.f, 0.f}, sgl::colors::GREEN},
        }
    };

    const auto vao = sgl::vertex_array::create_or_panic();

    const auto vbo = sgl::vertex_buffer::create_or_panic(vertices.data(), sizeof(vertices),GL_STATIC_DRAW);

    vao.attrib_pointer(
        vbo, 0, 3,GL_FLOAT,GL_FALSE, sizeof(vertex),SGL_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer(
        vbo, 1, 4,GL_UNSIGNED_BYTE,GL_TRUE, sizeof(vertex),SGL_OFFSET_OF(vertex, color)
    );

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    glLineWidth(5.0f);

    sgl::render::set_clear_color(sgl::colors::GRAY);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        vao.bind();

        glDrawArrays(GL_LINES, 0, 2);

        sgl::vertex_array::unbind();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
