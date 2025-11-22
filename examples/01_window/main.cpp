#include "sgl.h"

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr auto SCREEN_TITLE = __FILE__;

int main() {
    auto window = sgl::window_t::create_or_panic(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    sgl::render_t::set_clear_color(sgl::color::WHITE);

    while (!window.should_close()) {
        sgl::render_t::clear_color_buffer();

        window.swap_buffers();
        sgl::window_t::poll_events();
    }

    return EXIT_SUCCESS;
}
