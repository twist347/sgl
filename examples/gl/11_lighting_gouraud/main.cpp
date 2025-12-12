/*
    Controls:
      - W / S  - forward / backward
      - A / D  - right / left
      - Q / E  - down / up
      - MOUSE  - camera rotate (yaw/pitch)
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

static constexpr auto OBJECT_VS_PATH = "shaders/object.vert";
static constexpr auto OBJECT_FS_PATH = "shaders/object.frag";

static constexpr auto LIGHT_VS_PATH = "shaders/light.vert";
static constexpr auto LIGHT_FS_PATH = "shaders/light.frag";

using vec3 = glm::vec<3, sgl::gl_float>;

struct vertex {
    sgl::gl_float pos[3]{};
    sgl::gl_float normal[3]{};
};

static constexpr float MOVE_SPEED = 5.f; // units per second
static constexpr float MOVE_SENSE = 0.1f; // degrees per pixel

static constexpr std::array<vertex, 24> g_vertices = {
    {
        // back face (0..3)  z = -0.5
        {.pos = {-0.5f, -0.5f, -0.5f}, .normal = {0.f, 0.f, -1.f}},
        {.pos = {0.5f, -0.5f, -0.5f}, .normal = {0.f, 0.f, -1.f}},
        {.pos = {0.5f, 0.5f, -0.5f}, .normal = {0.f, 0.f, -1.f}},
        {.pos = {-0.5f, 0.5f, -0.5f}, .normal = {0.f, 0.f, -1.f}},

        // front face (4..7)  z = 0.5
        {.pos = {-0.5f, -0.5f, 0.5f}, .normal = {0.f, 0.f, 1.f}},
        {.pos = {0.5f, -0.5f, 0.5f}, .normal = {0.f, 0.f, 1.f}},
        {.pos = {0.5f, 0.5f, 0.5f}, .normal = {0.f, 0.f, 1.f}},
        {.pos = {-0.5f, 0.5f, 0.5f}, .normal = {0.f, 0.f, 1.f}},

        // left face (8..11)  x = -0.5
        {.pos = {-0.5f, -0.5f, -0.5f}, .normal = {-1.f, 0.f, 0.f}},
        {.pos = {-0.5f, -0.5f, 0.5f}, .normal = {-1.f, 0.f, 0.f}},
        {.pos = {-0.5f, 0.5f, 0.5f}, .normal = {-1.f, 0.f, 0.f}},
        {.pos = {-0.5f, 0.5f, -0.5f}, .normal = {-1.f, 0.f, 0.f}},

        // right face (12..15)  x = 0.5
        {.pos = {0.5f, -0.5f, -0.5f}, .normal = {1.f, 0.f, 0.f}},
        {.pos = {0.5f, -0.5f, 0.5f}, .normal = {1.f, 0.f, 0.f}},
        {.pos = {0.5f, 0.5f, 0.5f}, .normal = {1.f, 0.f, 0.f}},
        {.pos = {0.5f, 0.5f, -0.5f}, .normal = {1.f, 0.f, 0.f}},

        // bottom face (16..19)  y = -0.5
        {.pos = {-0.5f, -0.5f, -0.5f}, .normal = {0.f, -1.f, 0.f}},
        {.pos = {0.5f, -0.5f, -0.5f}, .normal = {0.f, -1.f, 0.f}},
        {.pos = {0.5f, -0.5f, 0.5f}, .normal = {0.f, -1.f, 0.f}},
        {.pos = {-0.5f, -0.5f, 0.5f}, .normal = {0.f, -1.f, 0.f}},

        // top face (20..23)  y = 0.5
        {.pos = {-0.5f, 0.5f, -0.5f}, .normal = {0.f, 1.f, 0.f}},
        {.pos = {0.5f, 0.5f, -0.5f}, .normal = {0.f, 1.f, 0.f}},
        {.pos = {0.5f, 0.5f, 0.5f}, .normal = {0.f, 1.f, 0.f}},
        {.pos = {-0.5f, 0.5f, 0.5f}, .normal = {0.f, 1.f, 0.f}},
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

static constexpr std::array g_cube_positions = {
    glm::vec3{0.f, 0.f, 0.f},
    glm::vec3{2.f, 0.f, 1.f},
    glm::vec3{-2.f, 0.f, 2.f},
    glm::vec3{0.f, 2.f, 3.f},
    glm::vec3{0.f, -2.f, 4.f},
};

static constexpr std::array g_cube_colors = {
    glm::vec3{1.f, 0.5f, 0.31f},
    glm::vec3{0.2f, 0.6f, 1.0f},
    glm::vec3{0.3f, 1.0f, 0.3f},
    glm::vec3{1.0f, 0.8f, 0.2f},
    glm::vec3{0.9f, 0.3f, 0.9f},
};

static glm::vec3 light_pos = {1.2f, 1.f, 2.f};

static constexpr auto U_VIEW = "u_view";
static constexpr auto U_MODEL = "u_model";
static constexpr auto U_PROJECTION = "u_projection";
static constexpr auto U_OBJECT_COLOR = "u_object_color";
static constexpr auto U_LIGHT_COLOR = "u_light_color";
static constexpr auto U_LIGHT_POS = "u_light_pos";
static constexpr auto U_VIEW_POS = "u_view_pos";

void handle_input(sgl::camera &cam, float dt);

void render_object(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam);

void render_light(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam);

void update_light_pos(const sgl::shader &shader, float dt);

int main() {
    const auto window = sgl::window::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);
    window.set_vsync(true);
    window.set_show_fps(true);
    window.set_cursor_enabled(false);

    auto cam = sgl::camera::create(
        45.f,
        static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
        0.1f,
        100.f
    );
    cam.set_move_speed(MOVE_SPEED);
    cam.set_sens(MOVE_SENSE);

    const auto obj_shader = sgl::shader::create_from_files_or_panic(OBJECT_VS_PATH, OBJECT_FS_PATH);
    const auto light_shader = sgl::shader::create_from_files_or_panic(LIGHT_VS_PATH, LIGHT_FS_PATH);

    const auto projection = cam.get_projection_mat();
    constexpr auto light_color = glm::vec3{1.f, 1.f, 1.f};

    obj_shader.use();
    SGL_VERIFY(obj_shader.set_uniform_mat4(U_PROJECTION, glm::value_ptr(projection)));
    SGL_VERIFY(obj_shader.set_uniform_vec3(U_LIGHT_COLOR, glm::value_ptr(light_color)));
    SGL_VERIFY(obj_shader.set_uniform_vec3(U_LIGHT_POS, glm::value_ptr(light_pos)));

    light_shader.use();
    SGL_VERIFY(light_shader.set_uniform_mat4(U_PROJECTION, glm::value_ptr(projection)));

    const auto obj_vao = sgl::vertex_array::create_or_panic();
    const auto light_vao = sgl::vertex_array::create_or_panic();
    const auto vbo = sgl::vertex_buffer::create_or_panic(
        g_vertices.data(), sizeof(g_vertices), GL_STATIC_DRAW
    );
    const auto ebo = sgl::element_buffer::create_or_panic(
        g_indices.data(), sizeof(g_indices), GL_UNSIGNED_INT, GL_STATIC_DRAW
    );

    obj_vao.bind();
    vbo.bind();

    obj_vao.attrib_pointer_and_enable(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, pos)
    );
    obj_vao.attrib_pointer_and_enable(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, normal)
    );

    ebo.bind();

    light_vao.bind();
    vbo.bind();
    light_vao.attrib_pointer_and_enable(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), SGL_PTR_OFFSET_OF(vertex, pos)
    );
    ebo.bind();

    sgl::vertex_buffer::unbind();
    sgl::vertex_array::unbind();

    sgl::render::set_clear_color(sgl::colors::gray);
    sgl::render::enable_depth_test(true);

    while (!window.should_close()) {
        sgl::render::clear_color_depth_buffer();

        const float dt = sgl::get_dt_f();

        handle_input(cam, dt);
        update_light_pos(obj_shader, dt);

        render_object(obj_shader, obj_vao, cam);
        render_light(light_shader, light_vao, cam);

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

    if (const auto [dx, dy] = sgl::input::mouse_dt(); dx != 0.0 || dy != 0.0) {
        cam.rotate(static_cast<float>(dx), static_cast<float>(-dy));
    }
}

void render_object(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam) {
    shader.use();

    const auto view = cam.get_view_mat();
    const auto pos = cam.get_pos();

    SGL_VERIFY(shader.set_uniform_mat4(U_VIEW, glm::value_ptr(view)));
    SGL_VERIFY(shader.set_uniform_vec3(U_VIEW_POS, glm::value_ptr(pos)));

    vao.bind();

    const float t = sgl::get_time_f();

    for (std::size_t i = 0; i < g_cube_positions.size(); ++i) {
        glm::mat4 model{1.f};

        model = glm::translate(model, g_cube_positions[i]);

        const float angle = glm::radians(20.f) * static_cast<float>(i) + t * 0.7f;
        model = glm::rotate(model, angle, glm::vec3(1.f, 1.f, 0.f));

        SGL_VERIFY(shader.set_uniform_vec3(U_OBJECT_COLOR, glm::value_ptr(g_cube_colors[i])));
        SGL_VERIFY(shader.set_uniform_mat4(U_MODEL, glm::value_ptr(model)));

        glDrawElements(
            GL_TRIANGLES,
            static_cast<sgl::gl_sizei>(g_indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }
}

void render_light(const sgl::shader &shader, const sgl::vertex_array &vao, const sgl::camera &cam) {
    shader.use();

    const auto view = cam.get_view_mat();
    SGL_VERIFY(shader.set_uniform_mat4(U_VIEW, glm::value_ptr(view)));

    auto model = glm::translate(glm::mat4(1.f), light_pos);
    model = glm::scale(model, glm::vec3(0.2f));

    SGL_VERIFY(shader.set_uniform_mat4(U_MODEL, glm::value_ptr(model)));

    vao.bind();
    glDrawElements(
        GL_TRIANGLES,
        static_cast<sgl::gl_sizei>(g_indices.size()),
        GL_UNSIGNED_INT,
        nullptr
    );
}

void update_light_pos(const sgl::shader &shader, float dt) {
    static float angle = 0.f;
    angle += dt;

    static constexpr float radius = 3.f;

    light_pos.x = std::cos(angle) * radius;
    light_pos.z = std::sin(angle) * radius;
    light_pos.y = 1.f;

    shader.use();
    SGL_VERIFY(shader.set_uniform_vec3(U_LIGHT_POS, glm::value_ptr(light_pos)));
}
