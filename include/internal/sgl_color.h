#pragma once

namespace sgl {
    struct color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    namespace colors {
        inline constexpr color BLACK{0, 0, 0, 255};
        inline constexpr color WHITE{255, 255, 255, 255};
        inline constexpr color RED{255, 0, 0, 255};
        inline constexpr color GREEN{0, 255, 0, 255};
        inline constexpr color BLUE{0, 0, 255, 255};
        inline constexpr color YELLOW{255, 255, 0, 255};
        inline constexpr color CYAN{0, 255, 255, 255};
        inline constexpr color MAGENTA{255, 0, 255, 255};
        inline constexpr color GRAY{128, 128, 128, 255};
        inline constexpr color LIGHT_GRAY{192, 192, 192, 255};
        inline constexpr color DARK_GRAY{64, 64, 64, 255};
    }
}
