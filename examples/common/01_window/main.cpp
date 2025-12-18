/*
create window
*/

#include "sgl.h"

static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;
static constexpr auto TITLE = __FILE__;

int main() {
    const auto window = sgl::window::create_try({.width = WIDTH, .height = HEIGHT, .title = TITLE});

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        window.swap_buffers();
        sgl::window::poll_events();
    }

    return EXIT_SUCCESS;
}
