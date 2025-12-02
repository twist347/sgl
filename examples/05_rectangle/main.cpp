#include "sgl.h"

#include <array>

#include "glad/glad.h"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::color color{};
};

int main() {
    const auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    constexpr std::array<vertex, 4> vertices = {
        {
            {.pos = {0.5f, 0.5f, 0.0f}, .color = sgl::colors::red},
            {.pos = {0.5f, -0.5f, 0.0f}, .color = sgl::colors::green},
            {.pos = {-0.5f, -0.5f, 0.0f}, .color = sgl::colors::blue},
            {.pos = {-0.5f, 0.5f, 0.0f}, .color = sgl::colors::magenta},
        }
    };

    constexpr std::array<sgl::gl_ushort, 6> indices = {
        {
            0, 1, 3,
            1, 2, 3
        }
    };

    const auto vao = sgl::vertex_array::create_or_panic();

    const auto vbo = sgl::vertex_buffer::create_or_panic(vertices.data(), sizeof(vertices),GL_STATIC_DRAW);

    const auto ebo = sgl::element_buffer::create_or_panic(
        indices.data(), sizeof(indices),GL_UNSIGNED_SHORT,GL_STATIC_DRAW
    );

    vao.bind();
    vbo.bind();

    vao.attrib_pointer(
        0, 3,GL_FLOAT,GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer(
        1, 4,GL_UNSIGNED_BYTE,GL_TRUE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, color)
    );

    ebo.bind();

    vao.set_element_buffer(ebo);

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        vao.bind();

        glDrawElements(GL_TRIANGLES, ebo.count(), ebo.type(), nullptr);

        sgl::vertex_array::unbind();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
