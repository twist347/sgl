# Simple graphics library

## In progress

## Features

- Window & context: `sgl::window`
- Buffers & vertex arrays: `sgl::vertex_buffer`, `sgl::element_buffer`, `sgl::vertex_array`
- Shaders & uniforms: `sgl::shader`
- 2D textures: `sgl::texture_2d`
- Colors & clear helpers: `sgl::color`, `sgl::render::set_clear_color`, `clear_color_buffer`
- Time: `sgl::get_time()`, `sgl::get_time_f()`
- Input: `sgl::input::is_key_down`, `is_key_pressed`, etc.
- Optional FPS display in window title

## Build with CMake

```
$ cmake -S . -B build
$ cmake --build build
```

## Requirements

- C++20 or newer

## Quick start

```cpp
#include "sgl.h"

int main() {
    auto window = sgl::window::create_or_panic(1920, 1080, "sgl demo");
    window.set_vsync(true);

    sgl::render::set_clear_color(sgl::colors::gray);

    while (!window.should_close()) {
        sgl::render::clear_color_buffer();

        // your rendering here

        window.swap_buffers();
        sgl::window::poll_events();
    }
}