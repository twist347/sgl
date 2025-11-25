#pragma once

#include <array>
#include <cstddef>
#include <cmath>

namespace sgl {
    template<typename T, std::size_t N>
    struct vec {
        std::array<T, N> data{};

        constexpr T &operator[](std::size_t i) noexcept { return data[i]; }
        constexpr const T &operator[](std::size_t i) const noexcept { return data[i]; }

        constexpr T *begin() noexcept { return data.data(); }
        constexpr T *end() noexcept { return data.data() + N; }
        constexpr const T *begin() const noexcept { return data.data(); }
        constexpr const T *end() const noexcept { return data.data() + N; }
        constexpr const T *cbegin() const noexcept { return data.data(); }
        constexpr const T *cend() const noexcept { return data.data() + N; }

        constexpr bool operator==(const vec &) const noexcept = default;

        constexpr bool operator!=(const vec &) const noexcept = default;
    };

    template<typename T, std::size_t N>
    constexpr vec<T, N> &operator+=(vec<T, N> &lhs, const vec<T, N> &rhs) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            lhs[i] += rhs[i];
        }
        return lhs;
    }

    template<typename T, std::size_t N>
    constexpr vec<T, N> &operator-=(vec<T, N> &lhs, const vec<T, N> &rhs) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            lhs[i] -= rhs[i];
        }
        return lhs;
    }

    template<typename T, std::size_t N>
    constexpr vec<T, N> operator+(vec<T, N> lhs, const vec<T, N> &rhs) noexcept {
        lhs += rhs;
        return lhs;
    }

    template<typename T, std::size_t N>
    constexpr vec<T, N> operator-(vec<T, N> lhs, const vec<T, N> &rhs) noexcept {
        lhs -= rhs;
        return lhs;
    }

    template<typename T, std::size_t N>
    constexpr vec<T, N> operator-(const vec<T, N> &v) noexcept {
        vec<T, N> r{};
        for (std::size_t i = 0; i < N; ++i) {
            r[i] = -v[i];
        }
        return r;
    }

    template<typename T, std::size_t N, typename S>
    constexpr vec<T, N> &operator*=(vec<T, N> &v, S s) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<T>(v[i] * s);
        }
        return v;
    }

    template<typename T, std::size_t N, typename S>
    constexpr vec<T, N> &operator/=(vec<T, N> &v, S s) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<T>(v[i] / s);
        }
        return v;
    }

    template<typename T, std::size_t N, typename S>
    constexpr vec<T, N> operator*(vec<T, N> v, S s) noexcept {
        v *= s;
        return v;
    }

    template<typename T, std::size_t N, typename S>
    constexpr vec<T, N> operator*(S s, vec<T, N> v) noexcept {
        v *= s;
        return v;
    }

    template<typename T, std::size_t N, typename S>
    constexpr vec<T, N> operator/(vec<T, N> v, S s) noexcept {
        v /= s;
        return v;
    }

    template<typename T, std::size_t N>
    constexpr T dot(const vec<T, N> &a, const vec<T, N> &b) noexcept {
        T sum = T{};
        for (std::size_t i = 0; i < N; ++i) {
            sum += a[i] * b[i];
        }
        return sum;
    }

    template<typename T, std::size_t N>
    constexpr T length_squared(const vec<T, N> &v) noexcept {
        return dot(v, v);
    }

    template<typename T, std::size_t N, typename R = double>
    constexpr auto length(const vec<T, N> &v) noexcept -> R {
        return std::sqrt(static_cast<R>(length_squared(v)));
    }

    template<typename T, std::size_t N>
    constexpr vec<T, N> normalize(const vec<T, N> &v) noexcept {
        const auto len = length(v);
        if (len == 0.0) {
            return v;
        }
        return v / len;
    }

    using vec2f = vec<float, 2>;
    using vec3f = vec<float, 3>;
    using vec4f = vec<float, 4>;

    using vec2i = vec<int, 2>;
    using vec3i = vec<int, 3>;
    using vec4i = vec<int, 4>;
}
