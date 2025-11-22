#pragma once

namespace sgl {
    struct color_t {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    namespace color {
        inline constexpr color_t BLACK{0, 0, 0, 255};
        inline constexpr color_t WHITE{255, 255, 255, 255};
        inline constexpr color_t RED{255, 0, 0, 255};
        inline constexpr color_t GREEN{0, 255, 0, 255};
        inline constexpr color_t BLUE{0, 0, 255, 255};
    }
}
