//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <type_traits>
#include "Constants.hpp"
#include "FloatType.hpp"
#include "XyzException.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class Vector
    {
    public:
        using ValueType = T;
        static constexpr size_t SIZE = N;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(std::initializer_list<T> v)
        {
            if (v.size() != SIZE)
                XYZ_THROW("Incorrect number of arguments.");
            std::copy(v.begin(), v.end(), std::begin(values));
        }

        explicit Vector(T const (& arr)[N]) noexcept
        {
            std::copy(std::begin(arr), std::end(arr), std::begin(values));
        }

        constexpr Vector(const Vector& other) noexcept
        {
            std::copy(std::begin(other.values), std::end(other.values),
                      std::begin(values));
        }

        Vector& operator=(const Vector& other)
        {
            if (this != &other)
            {
                std::copy(std::begin(other.values), std::end(other.values),
                          std::begin(values));
            }
            return *this;
        }

        constexpr T operator[](unsigned i) const
        {
            return values[i];
        }

        constexpr T& operator[](unsigned i)
        {
            return values[i];
        }

        T values[SIZE];
    };

    template <typename T>
    class Vector<T, 2>
    {
    public:
        using ValueType = T;
        static constexpr size_t SIZE = 2;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(T x, T y) noexcept
            : values{x, y}
        {}

        explicit constexpr Vector(T const (& arr)[2]) noexcept
        {
            std::copy(std::begin(arr), std::end(arr), std::begin(values));
        }

        constexpr Vector(const Vector& other) noexcept
        {
            std::copy(std::begin(other.values), std::end(other.values),
                      std::begin(values));
        }

        constexpr Vector& operator=(const Vector& other)
        {
            if (this != &other)
            {
                std::copy(std::begin(other.values), std::end(other.values),
                          std::begin(values));
            }
            return *this;
        }

        constexpr T operator[](unsigned i) const
        {
            return values[i];
        }

        constexpr T& operator[](unsigned i)
        {
            return values[i];
        }

        T values[2];
    };

    template <typename T>
    class Vector<T, 3>
    {
    public:
        using ValueType = T;
        static constexpr size_t SIZE = 3;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(T x, T y, T z) noexcept
            : values{x, y, z}
        {}

        explicit Vector(T const (& arr)[3]) noexcept
        {
            std::copy(std::begin(arr), std::end(arr), std::begin(values));
        }

        constexpr Vector(const Vector& other) noexcept
        {
            std::copy(std::begin(other.values), std::end(other.values),
                      std::begin(values));
        }

        constexpr Vector& operator=(const Vector& other)
        {
            if (this != &other)
            {
                std::copy(std::begin(other.values), std::end(other.values),
                          std::begin(values));
            }
            return *this;
        }

        constexpr T operator[](unsigned i) const
        {
            return values[i];
        }

        constexpr T& operator[](unsigned i)
        {
            return values[i];
        }

        T values[3];
    };

    template <typename T>
    class Vector<T, 4>
    {
    public:
        using ValueType = T;
        static constexpr size_t SIZE = 4;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(T x, T y, T z, T w) noexcept
            : values{x, y, z, w}
        {}

        explicit Vector(T const (& arr)[4]) noexcept
        {
            std::copy(std::begin(arr), std::end(arr), std::begin(values));
        }

        constexpr Vector(const Vector& other) noexcept
        {
            std::copy(std::begin(other.values), std::end(other.values),
                      std::begin(values));
        }

        constexpr Vector& operator=(const Vector& other)
        {
            if (this != &other)
            {
                std::copy(std::begin(other.values), std::end(other.values),
                          std::begin(values));
            }
            return *this;
        }

        constexpr T operator[](unsigned i) const
        {
            return values[i];
        }

        constexpr T& operator[](unsigned i)
        {
            return values[i];
        }

        T values[4];
    };

    template <typename T, unsigned N>
    constexpr T* begin(Vector<T, N>& v)
    {
        return std::begin(v.values);
    }

    template <typename T, unsigned N>
    constexpr T* end(Vector<T, N>& v)
    {
        return std::end(v.values);
    }

    template <typename T, unsigned N>
    constexpr const T* begin(const Vector<T, N>& v)
    {
        return std::begin(v.values);
    }

    template <typename T, unsigned N>
    constexpr const T* end(const Vector<T, N>& v)
    {
        return std::end(v.values);
    }

    template <typename T, typename S, unsigned N>
    constexpr bool operator==(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
        {
            if (u[i] != v[i])
                return false;
        }
        return true;
    }

    template <typename T, typename S, unsigned N>
    constexpr bool operator!=(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        return !(u == v);
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator+=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] += v[i];
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator+=(Vector<T, N>& u, T scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] += scalar;
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator-=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] -= v[i];
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator-=(Vector<T, N>& u, T scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] -= scalar;
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator*=(Vector<T, N>& u, T scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= scalar;
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator*=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= v[i];
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator/=(Vector<T, N>& u, T scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= scalar;
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator/=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= v[i];
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N> operator-(Vector<T, N> v)
    {
        for (unsigned i = 0; i < N; ++i)
            v[i] = -v[i];
        return v;
    }

    template <typename T, typename S, unsigned N>
    constexpr auto operator+(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        Vector<decltype(T() * S()), N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] + v[i];
        return w;
    }

    template <typename T, typename S, unsigned N>
    constexpr auto operator-(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        Vector<decltype(T() * S()), N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] - v[i];
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator/(const Vector<T, N>& u, T scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / scalar;
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator/(T scalar, const Vector<T, N>& u)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = scalar / u[i];
        return w;
    }

    template <typename T, unsigned N>
    Vector<T, N> operator/(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / v[i];
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator*(const Vector<T, N>& u, T scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * scalar;
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator*(T scalar, const Vector<T, N>& v)
    {
        return v * scalar;
    }

    template <typename T, unsigned N>
    constexpr auto operator*(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * v[i];
        return w;
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
    {
        const T* it = begin(v);
        os << "[" << *it;
        while (++it != end(v))
            os << ", " << *it;
        return os << "]";
    }

    template <typename T>
    constexpr Vector<T, 2> make_vector2(T x, T y)
    {
        return Vector<T, 2>({x, y});
    }

    template <typename T>
    constexpr Vector<T, 3> make_vector3(T x, T y, T z)
    {
        return Vector<T, 3>({x, y, z});
    }

    template <typename T, typename U>
    constexpr Vector<T, 3> make_vector3(const Vector<U, 2>& v, T z)
    {
        return Vector<T, 3>({T(v[0]), T(v[1]), z});
    }

    template <typename T>
    constexpr Vector<T, 4> make_vector4(T x, T y, T z, T w)
    {
        return Vector<T, 4>({x, y, z, w});
    }

    template <typename T, typename U>
    constexpr Vector<T, 4> make_vector4(const Vector<U, 3>& v, T w)
    {
        return Vector<T, 4>({T(v[0]), T(v[1]), T(v[2]), w});
    }

    template <typename T, typename U, unsigned N>
    Vector<T, N> vector_cast(const Vector<U, N>& v)
    {
        if constexpr (std::is_same<T, U>::value)
        {
            return v;
        }
        else
        {
            Vector<T, N> result;
            for (unsigned i = 0; i < N; ++i)
                result[i] = static_cast<T>(v[i]);
            return result;
        }
    }

    template <unsigned Index, typename T, unsigned N>
    T& get(Vector<T, N>& v)
    {
        static_assert(Index < N, "Incorrect vector index.");
        return v[Index];
    }

    template <unsigned Index, typename T, unsigned N>
    T get(const Vector<T, N>& v)
    {
        static_assert(Index < N, "Incorrect vector index.");
        return v[Index];
    }

    template <typename T, typename S, unsigned N>
    auto dot(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        decltype(T() * S()) result = 0;
        for (unsigned i = 0; i < N; ++i)
            result += u[i] * v[i];
        return result;
    }

    template <typename T, unsigned N>
    auto get_length_squared(const Vector<T, N>& v)
    {
        return dot(v, v);
    }

    template <typename T, unsigned N>
    auto get_length(const Vector<T, N>& v)
    {
        return std::sqrt(get_length_squared(v));
    }

    template <typename T>
    Vector<T, 2> get_normal(const Vector<T, 2>& v)
    {
        return make_vector2(-v[1], v[0]);
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
    bool are_equivalent(const Vector<T, N>& u, const Vector<T, N>& v, T = 0)
    {
        return u == v;
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool are_equivalent(const Vector<T, N>& u, const Vector<T, N>& v,
                        T margin = Constants<T>::DEFAULT_MARGIN)
    {
        return get_length_squared(u - v) <= margin;
    }

    template <typename T, unsigned N>
    auto get_cos_angle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return dot(u, v) / std::sqrt(get_length_squared(u) * get_length_squared(v));
    }

    /** @brief Returns the smallest angle between @a u and @a v.
      * @return A value in the range 0 <= angle <= pi.
      */
    template <typename T, unsigned N>
    auto get_angle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return std::acos(get_cos_angle(u, v));
    }

    template <typename T>
    auto get_ccw_angle(const Vector<T, 2>& u, const Vector<T, 2>& v)
    {
        auto angle = get_angle(u, v);
        if (dot(get_normal(u), v) >= 0)
            return angle;
        else
            return 2 * Constants<decltype(angle)>::PI - angle;
    }

    template <typename T>
    Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
    {
        return {a[1] * b[2] - a[2] * b[1],
                a[2] * b[0] - a[0] * b[2],
                a[0] * b[1] - a[1] * b[0]};
    }

    template <typename T, unsigned N>
    auto get_unit(const Vector<T, N>& v)
    {
        return v / get_length(v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& resize_update(Vector<T, N>& v, T new_length)
    {
        return v *= (new_length / get_length(v));
    }

    template <typename T, unsigned N>
    Vector<T, N> resize(const Vector<T, N>& v, T new_length)
    {
        return v * (new_length / length(v));
    }

    template <typename T, unsigned N>
    Vector<T, N>& clamp_update(Vector<T, N>& v, T min, T max)
    {
        for (auto i = 0u; i < N; ++i)
            clamp(v[i], min, max);
    }

    template <typename T, unsigned N>
    Vector<T, N> clamp(Vector<T, N> v, T min, T max)
    {
        clamp_update(v, min, max);
        return v;
    }

    template <typename T>
    Vector<T, 2> reflect(const Vector<T, 2>& v,
                         const Vector<T, 2>& mirror)
    {
        auto n = get_normal(mirror);
        return v - 2 * (v * n) * n;
    }

    template <typename T>
    Vector<T, 2> rotate(const Vector<T, 2>& v, double radians)
    {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return make_vector2(T(v[0] * c - v[1] * s), T(v[0] * s + v[1] * c));
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
    bool is_null(Vector<T, N>& v, T = 0)
    {
        return std::none_of(v.begin(), v.end(),
                            [](auto n){return n != 0;});
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool is_null(Vector<T, N>& v, T margin = Constants<T>::DEFAULT_MARGIN)
    {
        return std::none_of(v.begin(), v.end(),
                            [&](auto n) {return fabs(n) > margin;});
    }


    using Vector2I = Vector<int, 2>;
    using Vector2F = Vector<float, 2>;
    using Vector2D = Vector<double, 2>;

    using Vector3I = Vector<int, 3>;
    using Vector3F = Vector<float, 3>;
    using Vector3D = Vector<double, 3>;

    using Vector4I = Vector<int, 4>;
    using Vector4F = Vector<float, 4>;
    using Vector4D = Vector<double, 4>;
}

template <typename T, size_t N>
struct std::tuple_size<Xyz::Vector<T, N>>
    : std::integral_constant<size_t, N>
{};

template <size_t I, typename T, size_t N>
struct std::tuple_element<I, Xyz::Vector<T, N>>
{
    using type = T;
};
