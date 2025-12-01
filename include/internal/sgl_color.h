#pragma once

namespace sgl {
    struct color {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        unsigned char a = 255;

        constexpr color() = default;

        constexpr color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) noexcept
            : r{r}, g{g}, b{b}, a{a} {
        }

        constexpr bool operator==(const color &other) const = default;

        constexpr bool operator!=(const color &other) const = default;
    };

    static_assert(sizeof(color) == 4);

    namespace colors {
        inline constexpr color transparent{0, 0, 0, 255};
        inline constexpr color black{0, 0, 0, 255};
        inline constexpr color white{255, 255, 255, 255};
        inline constexpr color red{255, 0, 0, 255};
        inline constexpr color green{0, 255, 0, 255};
        inline constexpr color blue{0, 0, 255, 255};
        inline constexpr color yellow{255, 255, 0, 255};
        inline constexpr color cyan{0, 255, 255, 255};
        inline constexpr color magenta{255, 0, 255, 255};
        inline constexpr color gray{128, 128, 128, 255};
        inline constexpr color light_gray{192, 192, 192, 255};
        inline constexpr color dark_gray{64, 64, 64, 255};
    }
}
