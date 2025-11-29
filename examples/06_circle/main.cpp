#include "sgl.h"

#include <array>
#include <numbers>

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

    constexpr std::size_t NUM_SEGMENTS = 32;
    constexpr std::size_t VERT_COUNT = 1 + NUM_SEGMENTS + 1;

    std::array<vertex, VERT_COUNT> vertices{};

    vertices[0].pos[0] = 0.f;
    vertices[0].pos[1] = 0.f;
    vertices[0].pos[2] = 0.f;
    vertices[0].color = sgl::colors::BLUE;

    constexpr float radius = 0.5f;

    for (std::size_t i = 0; i <= NUM_SEGMENTS; ++i) {
        const float seg = static_cast<float>(i) / static_cast<float>(NUM_SEGMENTS);
        const float angle = seg * 2.f * std::numbers::pi_v<float>;

        const std::size_t idx = 1 + i;

        vertices[idx].pos[0] = std::cos(angle) * radius;
        vertices[idx].pos[1] = std::sin(angle) * radius;
        vertices[idx].pos[2] = 0.f;

        vertices[idx].color = sgl::colors::RED;
    }

    const auto vao = sgl::vertex_array::create_or_panic();

    const auto vbo = sgl::vertex_buffer::create_or_panic(vertices.data(), sizeof(vertices),GL_STATIC_DRAW);

    vao.attrib_pointer(
        vbo, 0, 3,GL_FLOAT,GL_FALSE, sizeof(vertex),SGL_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer(
        vbo, 1, 4,GL_UNSIGNED_BYTE,GL_TRUE, sizeof(vertex),SGL_OFFSET_OF(vertex, color)
    );

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    sgl::render::set_clear_color(sgl::colors::GRAY);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        shader.use();
        vao.bind();

        glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<sgl::gl_sizei>(VERT_COUNT));

        sgl::vertex_array::unbind();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
