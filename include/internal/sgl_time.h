#pragma once

namespace sgl {
    double time() noexcept;

    float time_f() noexcept;

    double dt() noexcept;

    float dt_f() noexcept;

    void new_frame_time() noexcept;
}
