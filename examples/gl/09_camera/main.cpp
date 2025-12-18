/*
      - W / S  - forward / backward
      - A / D  - right / left
      - Q / E  - down / up
      - MOUSE  - car rotate (yaw/pitch)
      - Z      - depth test
*/

#include "sgl.h"

#include <array>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;
static constexpr auto TITLE = __FILE__;

static constexpr auto VERTEX_SHADER_PATH = "shaders/shader.vert";
static constexpr auto FRAGMENT_SHADER_PATH = "shaders/shader.frag";

using vec3 = glm::vec<3, sgl::gl_float>;

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::color color{};
};

static constexpr float MOVE_SPEED = 5.f; // units per second
static constexpr float MOVE_SENSE = 0.1f; // degrees per pixel

static constexpr std::array<vertex, 24> g_vertices = {
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

static constexpr std::array<sgl::gl_uint, 36> g_indices = {
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

static constexpr std::array<vec3, 5> g_cubes_pos = {
    {
        {0.f, 0.f, 0.f},
        {2.f, 0.f, 1.f},
        {-2.f, 0.f, 2.f},
        {0.f, 2.f, 3.f},
        {0.f, -2.f, 4.f},
    }
};

static bool g_enable_depth_test = true;

static constexpr auto U_VIEW = "u_view";
static constexpr auto U_MODEL = "u_model";
static constexpr auto U_PROJECTION = "u_projection";

void handle_input(sgl::camera &cam, float dt);

void render_scene(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam);

int main() {
    const auto window = sgl::window::create_try({.width = WIDTH, .height = HEIGHT, .title = TITLE, .cursor_enabled = false});


    auto cam = sgl::camera::create(
        45.f,
        static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
        0.1f,
        100.f
    );
    cam.set_move_speed(MOVE_SPEED);
    cam.set_sens(MOVE_SENSE);

    const auto vao = sgl::vertex_array::create_try();
    const auto vbo = sgl::vertex_buffer::create_try(g_vertices.data(), sizeof(g_vertices), GL_STATIC_DRAW);
    const auto ebo = sgl::element_buffer::create_try(
        g_indices.data(), sizeof(g_indices), GL_UNSIGNED_INT, GL_STATIC_DRAW
    );

    vao.bind();
    vbo.bind();

    vao.attrib_pointer_and_enable(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, pos)
    );

    vao.attrib_pointer_and_enable(
        1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, color)
    );

    ebo.bind();

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    const auto shader = sgl::shader::create_from_files_try(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    shader.use();
    {
        const auto proj = cam.get_projection_mat();
        SGL_VERIFY(shader.set_uniform_mat4(U_PROJECTION, glm::value_ptr(proj)));
    }

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_depth_buffer();

        const float dt = sgl::get_dt_f();

        handle_input(cam, dt);
        render_scene(shader, vao, cam);

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}

void handle_input(sgl::camera &cam, float dt) {
    // WASD + Q/E
    if (sgl::input::is_key_down(sgl::key::w)) cam.move_forward(dt);
    if (sgl::input::is_key_down(sgl::key::s)) cam.move_backward(dt);

    if (sgl::input::is_key_down(sgl::key::d)) cam.move_right(dt);
    if (sgl::input::is_key_down(sgl::key::a)) cam.move_left(dt);

    if (sgl::input::is_key_down(sgl::key::e)) cam.move_up(dt);
    if (sgl::input::is_key_down(sgl::key::q)) cam.move_down(dt);

    // toggle depth test
    if (sgl::input::is_key_pressed(sgl::key::z)) {
        g_enable_depth_test = !g_enable_depth_test;
    }

    if (const auto [dx, dy] = sgl::input::mouse_dt(); dx != 0.0 || dy != 0.0) {
        cam.rotate(static_cast<float>(dx), static_cast<float>(-dy));
    }
}

void render_scene(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam) {
    sgl::render::enable_depth_test(g_enable_depth_test);

    const auto view = cam.get_view_mat();
    SGL_VERIFY(shader.set_uniform_mat4(U_VIEW, glm::value_ptr(view)));

    vao.bind();
    shader.use();

    for (std::size_t i = 0; i < g_cubes_pos.size(); ++i) {
        glm::mat4 model{1.f};

        const float t = sgl::get_time_f();
        const float angle = glm::radians(45.f) * t + glm::radians(20.f) * static_cast<float>(i);

        model = glm::translate(model, g_cubes_pos[i]);
        model = glm::rotate(model, angle, glm::vec3(1.f, 1.f, 1.f));

        SGL_VERIFY(shader.set_uniform_mat4(U_MODEL, glm::value_ptr(model)));

        glDrawElements(
            GL_TRIANGLES,
            static_cast<sgl::gl_sizei>(g_indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }
}
