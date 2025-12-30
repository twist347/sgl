/*
draw a point
*/

#include "sgl.h"

#include <array>

#include "glad/glad.h"

static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;
static constexpr auto TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::color color{};
};

int main() {
    const auto window = sgl::window::create_try({.width = WIDTH, .height = HEIGHT, .title = TITLE});

    constexpr std::array<vertex, 4> vertices = {
        {
            {.pos = {-0.5f, 0.5f, 0.f}, .color = sgl::colors::red},
            {.pos = {0.5f, 0.5f, 0.f}, .color = sgl::colors::green},
            {.pos = {-0.5f, -0.5f, 0.f}, .color = sgl::colors::blue},
            {.pos = {0.5f, -0.5f, 0.f}, .color = sgl::colors::yellow},
        }
    };

    const auto vao = sgl::vertex_array::create_try();

    const auto vbo = sgl::vertex_buffer::create_try(std::span{vertices}, GL_STATIC_DRAW);

    vao.bind();
    vbo.bind();

    vao.attrib_pointer_and_enable<vertex>(
        0, 3,GL_FLOAT,GL_FALSE, SGL_PTR_OFFSET_OF(vertex, pos)
    );
    vao.attrib_pointer_and_enable<vertex>(
        1, 4,GL_UNSIGNED_BYTE,GL_TRUE, SGL_PTR_OFFSET_OF(vertex, color)
    );

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    const auto shader = sgl::shader::create_from_files_try(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(30.f);

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        vao.bind();

        glDrawArrays(GL_POINTS, 0, vertices.size());

        sgl::vertex_array::unbind();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
