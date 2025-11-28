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
    sgl::gl_float pos[3];
    sgl::color color;
    sgl::gl_float tex[2];
};

int main() {
    auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    const auto texture = sgl::texture_2d::create_from_file_or_panic(TEXTURE_PATH);

    constexpr std::array<vertex, 4> vertices = {
        {
            {{0.5f, 0.5f, 0.0f}, sgl::colors::RED, {1.0f, 1.0f}}, // right top
            {{0.5f, -0.5f, 0.0f}, sgl::colors::GREEN, {1.0f, 0.0f}}, // right bottom
            {{-0.5f, -0.5f, 0.0f}, sgl::colors::BLUE, {0.0f, 0.0f}}, // left  bottom
            {{-0.5f, 0.5f, 0.0f}, sgl::colors::MAGENTA, {0.0f, 1.0f}}, // left  top
        }
    };

    constexpr std::array<sgl::gl_ushort, 6> indices = {
        {
            0, 1, 3,
            1, 2, 3
        }
    };

    auto vao = sgl::vertex_array::create_or_panic();

    const auto vbo = sgl::vertex_buffer::create_or_panic(
        vertices.data(),
        sizeof(vertices),
        GL_STATIC_DRAW
    );

    const auto ebo = sgl::element_buffer::create_or_panic(
        indices.data(),
        sizeof(indices),
        GL_UNSIGNED_SHORT,
        GL_STATIC_DRAW
    );

    vao.attrib_pointer(
        vbo,
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertex),
        SGL_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer(
        vbo,
        1,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(vertex),
        SGL_OFFSET_OF(vertex, color)
    );

    vao.attrib_pointer(
        vbo,
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertex),
        SGL_OFFSET_OF(vertex, tex)
    );

    vao.set_element_buffer(ebo);

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    constexpr sgl::gl_int v0 = 0;
    shader.set_uniform("tex0", &v0, sgl::shader_uniform_type::INT);

    sgl::render::set_clear_color(sgl::colors::GRAY);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        texture.bind(v0);
        vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
