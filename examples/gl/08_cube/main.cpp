/*
draw spinning cubes and spinning camera
*/

#include "sgl.h"

#include <array>
#include <cmath>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";

static constexpr auto CAM_RADIUS = 12.f;

using vec3 = glm::vec<3, sgl::gl_float>;

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::color color{};
};

static bool enable_depth_test = true;

static constexpr auto U_VIEW = "u_view";
static constexpr auto U_MODEL = "u_model";
static constexpr auto U_PROJECTION = "u_projection";

int main() {
    const auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    window.set_vsync(true);
    window.set_show_fps(true);

    constexpr std::array<vertex, 24> vertices = {
        {
            // back face
            {.pos = {-0.5f, -0.5f, -0.5f}, .color = sgl::colors::red},
            {.pos = {0.5f, -0.5f, -0.5f}, .color = sgl::colors::red},
            {.pos = {0.5f, 0.5f, -0.5f}, .color = sgl::colors::red},
            {.pos = {-0.5f, 0.5f, -0.5f}, .color = sgl::colors::red},

            // front face
            {.pos = {-0.5f, -0.5f, 0.5f}, .color = sgl::colors::green},
            {.pos = {0.5f, -0.5f, 0.5f}, .color = sgl::colors::green},
            {.pos = {0.5f, 0.5f, 0.5f}, .color = sgl::colors::green},
            {.pos = {-0.5f, 0.5f, 0.5f}, .color = sgl::colors::green},

            // left face
            {.pos = {-0.5f, -0.5f, -0.5f}, .color = sgl::colors::blue},
            {.pos = {-0.5f, -0.5f, 0.5f}, .color = sgl::colors::blue},
            {.pos = {-0.5f, 0.5f, 0.5f}, .color = sgl::colors::blue},
            {.pos = {-0.5f, 0.5f, -0.5f}, .color = sgl::colors::blue},

            // right face
            {.pos = {0.5f, -0.5f, -0.5f}, .color = sgl::colors::yellow},
            {.pos = {0.5f, -0.5f, 0.5f}, .color = sgl::colors::yellow},
            {.pos = {0.5f, 0.5f, 0.5f}, .color = sgl::colors::yellow},
            {.pos = {0.5f, 0.5f, -0.5f}, .color = sgl::colors::yellow},

            // bottom face
            {.pos = {-0.5f, -0.5f, -0.5f}, .color = sgl::colors::cyan},
            {.pos = {0.5f, -0.5f, -0.5f}, .color = sgl::colors::cyan},
            {.pos = {0.5f, -0.5f, 0.5f}, .color = sgl::colors::cyan},
            {.pos = {-0.5f, -0.5f, 0.5f}, .color = sgl::colors::cyan},

            // top face
            {.pos = {-0.5f, 0.5f, -0.5f}, .color = sgl::colors::magenta},
            {.pos = {0.5f, 0.5f, -0.5f}, .color = sgl::colors::magenta},
            {.pos = {0.5f, 0.5f, 0.5f}, .color = sgl::colors::magenta},
            {.pos = {-0.5f, 0.5f, 0.5f}, .color = sgl::colors::magenta},
        }
    };

    constexpr std::array<sgl::gl_uint, 36> indices = {
        {
            // back face
            0, 1, 2,
            2, 3, 0,

            // front face
            4, 5, 6,
            6, 7, 4,

            // left face
            8, 9, 10,
            10, 11, 8,

            // right face
            12, 13, 14,
            14, 15, 12,

            // bottom face
            16, 17, 18,
            18, 19, 16,

            // top face
            20, 21, 22,
            22, 23, 20
        }
    };

    constexpr std::array<vec3, 5> cubes_pos = {
        {
            {0.f, 0.f, 0.f},
            {2.f, 0.f, 1.f},
            {-2.f, 0.f, 2.f},
            {0.f, 2.f, 3.f},
            {0.f, -2.f, 4.f},
        }
    };

    const auto vao = sgl::vertex_array::create_or_panic();

    const auto vbo = sgl::vertex_buffer::create_or_panic(vertices.data(), sizeof(vertices),GL_STATIC_DRAW);

    const auto ebo = sgl::element_buffer::create_or_panic(
        indices.data(), sizeof(indices),GL_UNSIGNED_INT,GL_STATIC_DRAW
    );

    vao.bind();
    vbo.bind();

    vao.attrib_pointer_and_enable(
        0, 3,GL_FLOAT,GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer_and_enable(
        1, 4,GL_UNSIGNED_BYTE,GL_TRUE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, color)
    );

    ebo.bind();

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    const auto shader = sgl::shader::create_from_files_or_panic(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    constexpr auto cam_target = glm::vec3(0.f, 0.f, 0.f);
    constexpr auto cam_up = glm::vec3(0.f, 1.f, 0.f);

    auto projection = glm::perspective(
        glm::radians(45.f),
        static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
        0.1f, 100.f
    );

    shader.use();
    SGL_VERIFY(shader.set_uniform_mat4(U_PROJECTION, glm::value_ptr(projection)));

    sgl::render::set_clear_color(sgl::colors::gray);

    sgl::render::enable_depth_test(enable_depth_test);

    while (!window.should_close()) {
        sgl::render::clear_color_depth_buffer();

        if (sgl::input::is_key_pressed(sgl::key::d)) {
            enable_depth_test = !enable_depth_test;
        }

        sgl::render::enable_depth_test(enable_depth_test);

        const auto time = sgl::get_time_f();

        const auto cam_pos = glm::vec3(std::sin(time) * CAM_RADIUS, 0.f, std::cos(time) * CAM_RADIUS);
        auto view = glm::lookAt(cam_pos, cam_target, cam_up);

        shader.use();
        vao.bind();

        SGL_VERIFY(shader.set_uniform_mat4(U_VIEW, glm::value_ptr(view)));

        for (std::size_t i = 0; i < cubes_pos.size(); ++i) {
            glm::mat4 model{1.f};

            const float angle = glm::radians(45.f) * time + glm::radians(20.f) * static_cast<float>(i);

            model = glm::translate(model, cubes_pos[i]);
            model = glm::rotate(model, angle, glm::vec3(1.f, 1.f, 1.f));

            SGL_VERIFY(shader.set_uniform_mat4(U_MODEL, glm::value_ptr(model)));
            glDrawElements(GL_TRIANGLES, static_cast<sgl::gl_sizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
        }

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
