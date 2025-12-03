/*
draw a rect with texture
*/

#include "sgl.h"

#include <array>

#include "glad/glad.h"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";
static constexpr auto TEXTURE_PATH = "textures/img1.png";

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::color color{};
    sgl::gl_float tex[2]{};
};

int main() {
    const auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    window.set_vsync(true);

    const auto texture = sgl::texture_2d::create_from_file_or_panic(TEXTURE_PATH);

    constexpr std::array<vertex, 4> vertices = {
        {
            {.pos = {0.5f, 0.5f, 0.f}, .color = sgl::colors::red, .tex = {1.f, 1.f}}, // right top
            {.pos = {0.5f, -0.5f, 0.f}, .color = sgl::colors::green, .tex = {1.f, 0.f}}, // right bottom
            {.pos = {-0.5f, -0.5f, 0.f}, .color = sgl::colors::blue, .tex = {0.f, 0.f}}, // left bottom
            {.pos = {-0.5f, 0.5f, 0.f}, .color = sgl::colors::magenta, .tex = {0.f, 1.f}}, // left top
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

    vao.attrib_pointer(
        2, 2,GL_FLOAT,GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, tex)
    );

    ebo.bind();

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    shader.use();
    constexpr sgl::gl_int v0 = 0;
    SGL_VERIFY(shader.set_uniform("u_tex0", v0));

    texture.bind(v0);

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        vao.bind();
        glDrawElements(GL_TRIANGLES, static_cast<sgl::gl_sizei>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
