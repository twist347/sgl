#pragma once

namespace sgl {
    enum class key : std::size_t {
        unknown = 0,

        space,
        apostrophe,
        comma,
        minus,
        period,
        slash,

        num0, num1, num2, num3, num4, num5, num6, num7, num8, num9,

        a, b, c, d, e, f, g,
        h, i, j, k, l, m, n,
        o, p, q, r, s, t, u,
        v, w, x, y, z,

        escape,
        enter,
        tab,
        backspace,
        insert,
        del,

        right,
        left,
        down,
        up,

        count
    };

    enum class mouse_button : std::size_t {
        unknown = 0,

        left,
        right,
        middle,

        count
    };
}
