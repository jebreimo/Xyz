//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <type_traits>

#include "Clamp.hpp"
#include "Constants.hpp"
#include "XyzException.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    struct Vector
    {
        static_assert(N > 0, "N must be greater than 0.");

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

        explicit Vector(T const (&arr)[N]) noexcept
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
    struct Vector<T, 2>
    {
        using ValueType = T;
        static constexpr size_t SIZE = 2;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(T x, T y) noexcept
            : values{x, y}
        {}

        template <typename U,
                  typename = std::enable_if_t<std::is_arithmetic_v<U>>>
        explicit constexpr Vector(const std::pair<U, U>& p) noexcept
            : values{T(p.first), T(p.second)}
        {}

        explicit constexpr Vector(T const (&arr)[2]) noexcept
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

        constexpr const T& x() const noexcept
        {
            return values[0];
        }

        constexpr T& x() noexcept
        {
            return values[0];
        }

        constexpr const T& y() const noexcept
        {
            return values[1];
        }

        constexpr T& y() noexcept
        {
            return values[1];
        }

        T values[2];
    };

    template <typename T>
    struct Vector<T, 3>
    {
        using ValueType = T;
        static constexpr size_t SIZE = 3;

        constexpr Vector() noexcept
            : values()
        {}

        constexpr Vector(T x, T y, T z) noexcept
            : values{x, y, z}
        {}

        explicit constexpr Vector(T const (&arr)[3]) noexcept
            : values{arr[0], arr[1], arr[2]}
        {}

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

        constexpr const T& x() const noexcept
        {
            return values[0];
        }

        constexpr T& x() noexcept
        {
            return values[0];
        }

        constexpr const T& y() const noexcept
        {
            return values[1];
        }

        constexpr T& y() noexcept
        {
            return values[1];
        }

        constexpr const T& z() const noexcept
        {
            return values[2];
        }

        constexpr T& z() noexcept
        {
            return values[2];
        }

        T values[3];
    };

    template <typename T>
    struct Vector<T, 4>
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

        explicit Vector(T const (&arr)[4]) noexcept
            : values{arr[0], arr[1], arr[2], arr[3]}
        {}

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

        constexpr const T& x() const noexcept
        {
            return values[0];
        }

        constexpr T& x() noexcept
        {
            return values[0];
        }

        constexpr const T& y() const noexcept
        {
            return values[1];
        }

        constexpr T& y() noexcept
        {
            return values[1];
        }

        constexpr const T& z() const noexcept
        {
            return values[2];
        }

        constexpr T& z() noexcept
        {
            return values[2];
        }

        constexpr const T& w() const noexcept
        {
            return values[3];
        }

        constexpr T& w() noexcept
        {
            return values[3];
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
    constexpr Vector<T, N>& operator+=(Vector<T, N>& u,
                                       std::type_identity_t<T> scalar)
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
    constexpr Vector<T, N>& operator-=(Vector<T, N>& u,
                                       std::type_identity_t<T> scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] -= scalar;
        return u;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>& operator*=(Vector<T, N>& u,
                                       std::type_identity_t<T> scalar)
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
    constexpr Vector<T, N>& operator/=(Vector<T, N>& u,
                                       std::type_identity_t<T> scalar)
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
    constexpr auto operator/(const Vector<T, N>& u,
                             std::type_identity_t<T> scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / scalar;
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator/(std::type_identity_t<T> scalar,
                             const Vector<T, N>& u)
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
    constexpr auto operator*(const Vector<T, N>& u,
                             std::type_identity_t<T> scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * scalar;
        return w;
    }

    template <typename T, unsigned N>
    constexpr auto operator*(std::type_identity_t<T> scalar,
                             const Vector<T, N>& v)
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
    [[nodiscard]]
    constexpr Vector<T, 2> make_vector2(T x, T y)
    {
        return Vector<T, 2>({x, y});
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 3> make_vector3(T x, T y, T z)
    {
        return Vector<T, 3>({x, y, z});
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 3> make_vector3(const Vector<T, 2>& v,
                                        std::type_identity_t<T> z)
    {
        return Vector<T, 3>({v[0], v[1], z});
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 4> make_vector4(T x, T y, T z, T w)
    {
        return Vector<T, 4>({x, y, z, w});
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 4> make_vector4(const Vector<T, 3>& v,
                                        std::type_identity_t<T> w)
    {
        return Vector<T, 4>({v[0], v[1], v[2], w});
    }

    template <typename T, typename U, unsigned N>
    [[nodiscard]]
    constexpr Vector<T, N> vector_cast(const Vector<U, N>& v)
    {
        if constexpr (std::is_same_v<T, U>)
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
    [[nodiscard]]
    T& get(Vector<T, N>& v)
    {
        static_assert(Index < N, "Incorrect vector index.");
        return v[Index];
    }

    template <unsigned Index, typename T, unsigned N>
    [[nodiscard]]
    T get(const Vector<T, N>& v)
    {
        static_assert(Index < N, "Incorrect vector index.");
        return v[Index];
    }

    template <typename T, typename S, unsigned N>
    [[nodiscard]]
    constexpr auto dot(const Vector<T, N>& u, const Vector<S, N>& v)
    {
        decltype(T() * S()) result = 0;
        for (unsigned i = 0; i < N; ++i)
            result += u[i] * v[i];
        return result;
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
    {
        return {
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]
        };
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr auto get_length_squared(const Vector<T, N>& v)
    {
        return dot(v, v);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    auto get_length(const Vector<T, N>& v)
    {
        return std::sqrt(get_length_squared(v));
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 2> get_normal(const Vector<T, 2>& v)
    {
        return make_vector2(-v[1], v[0]);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    bool are_equal(const Vector<T, N>& u, const Vector<T, N>& v,
                   std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return get_length_squared(u - v) <= margin;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    auto get_cos_angle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return dot(u, v) / std::sqrt(get_length_squared(u) * get_length_squared(v));
    }

    /** @brief Returns the smallest angle between @a u and @a v.
      * @return A value in the range 0 <= angle <= pi.
      */
    template <typename T, unsigned N>
    [[nodiscard]]
    auto get_angle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return std::acos(get_cos_angle(u, v));
    }

    template <typename T>
    [[nodiscard]]
    auto get_ccw_angle(const Vector<T, 2>& u, const Vector<T, 2>& v)
    {
        auto angle = get_angle(u, v);
        if (dot(get_normal(u), v) >= 0)
            return angle;
        return 2 * Constants<decltype(angle)>::PI - angle;
    }

    /**
     * @brief Returns the vector formed by projecting @a v onto the plane
     * defined by the normal vector @a normal.
     * @param v The vector to project.
     * @param normal The normal vector of the plane.
     * @return The projection of @a v onto the plane.
     */
    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, N> get_projection(const Vector<T, N>& v,
                                const Vector<T, N>& normal)
    {
        auto n = normalize(normal);
        return v - dot(v, n) * n;
    }

    /**
     * @brief Returns the counter-clockwise angle between the vectors
     * @a u and @a v in the plane defined by the normal vector @a normal.
     * @param u The first vector.
     * @param v The second vector.
     * @param normal The normal vector of the plane.
     * @return A value in the range 0 <= angle < 2 * pi.
     */
    template <typename T>
    [[nodiscard]]
    auto get_ccw_angle(const Vector<T, 3>& u, const Vector<T, 3>& v, const Vector<T, 3>& normal)
    {
        auto angle = get_angle(get_projection(u, normal),
                               get_projection(v, normal));

        if (dot(normal, cross(u, v)) >= 0)
            return angle;

        return 2 * Constants<decltype(angle)>::PI - angle;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    auto normalize(const Vector<T, N>& v)
    {
        return v / get_length(v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& scale_inplace(Vector<T, N>& v,
                                std::type_identity_t<T> new_length)
    {
        return v *= new_length / get_length(v);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, N> get_scaled(const Vector<T, N>& v,
                            std::type_identity_t<T> new_length)
    {
        return v * (new_length / get_length(v));
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>&
    clamp_inplace(Vector<T, N>& v,
                  std::type_identity_t<T> min,
                  std::type_identity_t<T> max)
    {
        for (unsigned i = 0; i < N; ++i)
            v[i] = Xyz::clamp(v[i], min, max);
        return v;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr Vector<T, N>
    get_clamped(Vector<T, N> v,
                std::type_identity_t<T> min,
                std::type_identity_t<T> max)
    {
        clamp_inplace(v, min, max);
        return v;
    }

    template <typename T, unsigned N>
    constexpr Vector<T, N>&
    clamp_inplace(Vector<T, N>& v,
                  const Vector<std::type_identity_t<T>, N>& min,
                  const Vector<std::type_identity_t<T>, N>& max)
    {
        for (unsigned i = 0; i < N; ++i)
            v[i] = Xyz::clamp(v[i], min[i], max[i]);
        return v;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, N> get_clamped(Vector<T, N> v,
                             const Vector<std::type_identity_t<T>, N>& min,
                             const Vector<std::type_identity_t<T>, N>& max)
    {
        clamp_inplace(v, min, max);
        return v;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> reflect(const Vector<T, 2>& v,
                         const Vector<T, 2>& mirror)
    {
        auto n = get_normal(mirror);
        return v - 2 * (v * n) * n;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> rotate(const Vector<T, 2>& v, double radians)
    {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return make_vector2(T(v[0] * c - v[1] * s), T(v[0] * s + v[1] * c));
    }

    template <typename T, unsigned N,
              std::enable_if_t<std::is_integral_v<T>, int>  = 0>
    [[nodiscard]]
    bool is_null(Vector<T, N>& v, T = 0)
    {
        return std::none_of(v.begin(), v.end(),
                            [](auto n) { return n != 0; });
    }

    template <typename T, unsigned N,
              std::enable_if_t<std::is_floating_point_v<T>, int>  = 0>
    [[nodiscard]]
    bool is_null(Vector<T, N>& v,
                 std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return std::none_of(v.begin(), v.end(),
                            [&](auto n) { return fabs(n) > margin; });
    }

    template <typename T, unsigned N,
              std::enable_if_t<std::is_floating_point_v<T>, int>  = 0>
    [[nodiscard]]
    Vector<T, N> floor(const Vector<T, N>& v)
    {
        Vector<T, N> result;
        for (unsigned i = 0; i < N; ++i)
            result[i] = std::floor(v[i]);
        return result;
    }

    template <typename T, unsigned N,
              std::enable_if_t<std::is_floating_point_v<T>, int>  = 0>
    [[nodiscard]]
    Vector<T, N> ceil(const Vector<T, N>& v)
    {
        Vector<T, N> result;
        for (unsigned i = 0; i < N; ++i)
            result[i] = std::ceil(v[i]);
        return result;
    }

    /**
     * @brief Adds a new coordinate to the end of a vector, expanding its
     *  dimension by one.
     */
    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr Vector<T, N + 1> push_back(const Vector<T, N>& v,
                                         std::type_identity_t<T> value)
    {
        Vector<T, N + 1> result;
        for (unsigned i = 0; i < N; ++i)
            result[i] = v[i];
        result[N] = value;
        return result;
    }

    /**
     * @brief Removes the last coordinate from a vector, shrinking its
     *  dimension by one.
     */
    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr Vector<T, N - 1> drop_back(const Vector<T, N>& v)
    {
        Vector<T, N - 1> result;
        for (unsigned i = 0; i < N - 1; ++i)
            result[i] = v[i];
        return result;
    }

    /**
     * @brief Returns a vector with the same coordinates as @a v, but with
     *  the dimension changed to @a SIZE.
     */
    template <unsigned SIZE, typename T, unsigned N>
    [[nodiscard]]
    constexpr Vector<T, SIZE> resize(const Vector<T, N>& v)
    {
        Vector<T, SIZE> result;
        for (unsigned i = 0; i < std::min(SIZE, N); ++i)
            result[i] = v[i];
        return result;
    }

    /**
     * @brief Converts a vector to homogeneous coordinates by adding a
     *  coordinate with value 1 at the end.
     */
    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    Vector<T, N + 1> to_hg(const Vector<T, N>& v)
    {
        return push_back(v, 1);
    }

    /**
     * @brief Converts a vector from homogeneous coordinates by removing the
     *  last coordinate. If the last coordinate is not 1, the result is
     *  divided by the removed coordinate.
     * @throws XyzException if the last coordinate is zero.
     */
    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    Vector<T, N - 1> from_hg(const Vector<T, N>& v)
    {
        if (v[N - 1] == 1)
            return drop_back(v);

        if (v[N - 1] == 0)
            XYZ_THROW("Cannot convert from homogeneous coordinates: last coordinate is zero.");

        return drop_back(v) / v[N - 1];
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

template <typename T, unsigned N>
struct std::tuple_size<Xyz::Vector<T, N>>
    : std::integral_constant<unsigned, N>
{};

template <size_t I, typename T, unsigned N>
struct std::tuple_element<I, Xyz::Vector<T, N>>
{
    using type = T;
};
