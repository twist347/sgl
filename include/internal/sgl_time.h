#pragma once

namespace sgl {
    double get_time() noexcept;

    float get_time_f() noexcept;

    double get_dt() noexcept;

    float get_dt_f() noexcept;

    void new_frame_time() noexcept;
}