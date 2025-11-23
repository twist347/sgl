#pragma once

#include <array>
#include <cstddef>
#include <cmath>

namespace sgl {
    template<typename T, std::size_t N>
    struct vec {
        std::array<T, N> data{};

        T &operator[](std::size_t i) noexcept { return data[i]; }
        const T &operator[](std::size_t i) const noexcept { return data[i]; }

        T *begin() noexcept { return data.data(); }
        T *end() noexcept { return data.data() + N; }
        const T *begin() const noexcept { return data.data(); }
        const T *end() const noexcept { return data.data() + N; }
        const T *cbegin() const noexcept { return data.data(); }
        const T *cend() const noexcept { return data.data() + N; }

        bool operator==(const vec &) const noexcept = default;

        bool operator!=(const vec &) const noexcept = default;
    };

    template<typename T, std::size_t N>
    vec<T, N> &operator+=(vec<T, N> &lhs, const vec<T, N> &rhs) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            lhs[i] += rhs[i];
        }
        return lhs;
    }

    template<typename T, std::size_t N>
    vec<T, N> &operator-=(vec<T, N> &lhs, const vec<T, N> &rhs) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            lhs[i] -= rhs[i];
        }
        return lhs;
    }

    template<typename T, std::size_t N>
    vec<T, N> operator+(vec<T, N> lhs, const vec<T, N> &rhs) noexcept {
        lhs += rhs;
        return lhs;
    }

    template<typename T, std::size_t N>
    vec<T, N> operator-(vec<T, N> lhs, const vec<T, N> &rhs) noexcept {
        lhs -= rhs;
        return lhs;
    }

    template<typename T, std::size_t N>
    vec<T, N> operator-(const vec<T, N> &v) noexcept {
        vec<T, N> r{};
        for (std::size_t i = 0; i < N; ++i) {
            r[i] = -v[i];
        }
        return r;
    }

    template<typename T, std::size_t N, typename S>
    vec<T, N> &operator*=(vec<T, N> &v, S s) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<T>(v[i] * s);
        }
        return v;
    }

    template<typename T, std::size_t N, typename S>
    vec<T, N> &operator/=(vec<T, N> &v, S s) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<T>(v[i] / s);
        }
        return v;
    }

    template<typename T, std::size_t N, typename S>
    vec<T, N> operator*(vec<T, N> v, S s) noexcept {
        v *= s;
        return v;
    }

    template<typename T, std::size_t N, typename S>
    vec<T, N> operator*(S s, vec<T, N> v) noexcept {
        v *= s;
        return v;
    }

    template<typename T, std::size_t N, typename S>
    vec<T, N> operator/(vec<T, N> v, S s) noexcept {
        v /= s;
        return v;
    }

    template<typename T, std::size_t N>
    T dot(const vec<T, N> &a, const vec<T, N> &b) noexcept {
        T sum = T{};
        for (std::size_t i = 0; i < N; ++i) {
            sum += a[i] * b[i];
        }
        return sum;
    }

    template<typename T, std::size_t N>
    T length_squared(const vec<T, N> &v) noexcept {
        return dot(v, v);
    }

    template<typename T, std::size_t N>
    auto length(const vec<T, N> &v) noexcept {
        using std::sqrt;
        return sqrt(static_cast<double>(length_squared(v)));
    }

    template<typename T, std::size_t N>
    vec<T, N> normalize(const vec<T, N> &v) noexcept {
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
