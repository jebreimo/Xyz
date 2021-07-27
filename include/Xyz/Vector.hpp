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

        static constexpr unsigned size()
        {
            return N;
        }

        constexpr Vector() noexcept
            : m_Values()
        {}

        constexpr Vector(std::initializer_list<T> v)
        {
            if (v.size() != N)
                XYZ_THROW("Incorrect number of arguments.");
            auto it = v.begin();
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = *it++;
        }

        explicit Vector(T (& arr)[N]) noexcept
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = arr[i];
        }

        template <typename U>
        Vector(const U* values, size_t count)
        {
            if (count != size())
                XYZ_THROW("Incorrect number of values.");

            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = U(values[i]);
        }

        Vector(const Vector& other) noexcept
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = other[i];
        }

        Vector& operator=(T (& arr)[N])
        {
            std::copy(arr, arr + N, begin());
            return *this;
        }

        Vector& operator=(const Vector<T, N>& other)
        {
            if (this != &other)
                std::copy(other.begin(), other.end(), begin());
            return *this;
        }

        constexpr const T& operator[](unsigned i) const
        {
            return m_Values[i];
        }

        constexpr T& operator[](unsigned i)
        {
            return m_Values[i];
        }

        constexpr const T* begin() const
        {
            return m_Values;
        }

        constexpr T* begin()
        {
            return m_Values;
        }

        constexpr const T* end() const
        {
            return m_Values + N;
        }

        constexpr T* end()
        {
            return m_Values + N;
        }

        constexpr T* data()
        {
            return m_Values;
        }

        constexpr const T* data() const
        {
            return m_Values;
        }

    private:
        T m_Values[N];
    };

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

    template <typename T, typename S, unsigned N>
    constexpr Vector<T, N>& operator*=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= scalar;
        return u;
    }

    template <typename T, typename S, unsigned N>
    constexpr Vector<T, N>& operator/=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= scalar;
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

    template <typename T, typename S, unsigned N>
    constexpr auto operator/(const Vector<T, N>& u, S scalar)
    {
        Vector<decltype(T() * S()), N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / scalar;
        return w;
    }

    template <typename T, typename S, unsigned N>
    constexpr auto operator*(const Vector<T, N>& u, S scalar)
    {
        Vector<decltype(T() * S()), N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * scalar;
        return w;
    }

    template <typename T, typename S, unsigned N>
    constexpr auto operator*(S scalar, const Vector<T, N>& v)
    {
        return v * scalar;
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
    {
        const T* it = v.begin();
        os << "[" << *it;
        while (++it != v.end())
            os << ", " << *it;
        return os << "]";
    }

    template <typename T>
    constexpr Vector<T, 2> makeVector2(T x, T y)
    {
        return Vector<T, 2>({x, y});
    }

    template <typename T>
    constexpr Vector<T, 3> makeVector3(T x, T y, T z)
    {
        return Vector<T, 3>({x, y, z});
    }

    template <typename T, typename U>
    constexpr Vector<T, 3> makeVector3(const Vector<U, 2>& v, T z)
    {
        return Vector<T, 3>({T(v[0]), T(v[1]), z});
    }

    template <typename T>
    constexpr Vector<T, 4> makeVector4(T x, T y, T z, T w)
    {
        return Vector<T, 4>({x, y, z, w});
    }

    template <typename T, typename U>
    constexpr Vector<T, 4> makeVector4(const Vector<U, 3>& v, T w)
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
    Vector<T, N>& divideAssign(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= v[i];
        return u;
    }

    template <typename T, unsigned N>
    Vector<T, N>& multiplyAssign(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= v[i];
        return u;
    }

    template <typename T, unsigned N>
    Vector<T, N> divide(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / v[i];
        return w;
    }

    template <typename T, typename U, unsigned N>
    Vector<T, N> divide(U scalar, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = scalar / v[i];
        return w;
    }

    template <typename T, unsigned N>
    Vector<T, N> multiply(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * v[i];
        return w;
    }

    template <typename T, unsigned N>
    auto getLengthSquared(const Vector<T, N>& v)
    {
        return dot(v, v);
    }

    template <typename T, unsigned N>
    auto getLength(const Vector<T, N>& v)
    {
        return std::sqrt(getLengthSquared(v));
    }

    template <typename T>
    Vector<T, 2> getNormal(const Vector<T, 2>& v)
    {
        return makeVector2(-v[1], v[0]);
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
    bool areEquivalent(const Vector<T, N>& u, const Vector<T, N>& v, T = 0)
    {
        return u == v;
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool areEquivalent(const Vector<T, N>& u, const Vector<T, N>& v,
                       T margin = Constants<T>::DEFAULT_MARGIN)
    {
        return getLengthSquared(u - v) <= margin;
    }

    template <typename T, unsigned N>
    auto getCosAngle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return dot(u, v) / std::sqrt(getLengthSquared(u) * getLengthSquared(v));
    }

    /** @brief Returns the smallest angle between @a u and @a v.
      * @return A value in the range 0 <= angle <= pi.
      */
    template <typename T, unsigned N>
    auto getAngle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return std::acos(getCosAngle(u, v));
    }

    template <typename T>
    auto getCcwAngle(const Vector<T, 2>& u, const Vector<T, 2>& v)
    {
        auto angle = getAngle(u, v);
        if (dot(getNormal(u), v) >= 0)
            return angle;
        else
            return 2 * Constants<decltype(angle)>::PI - angle;
    }

    template <typename T>
    Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
    {
        return makeVector3(a[1] * b[2] - a[2] * b[1],
                           a[2] * b[0] - a[0] * b[2],
                           a[0] * b[1] - a[1] * b[0]);
    }

    template <typename T, unsigned N>
    auto getUnit(const Vector<T, N>& v)
    {
        return v / getLength(v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& resizeUpdate(Vector<T, N>& v, T newLength)
    {
        return v *= (newLength / getLength(v));
    }

    template <typename T, unsigned N>
    Vector<T, N> resize(const Vector<T, N>& v, T newLength)
    {
        return v * (newLength / length(v));
    }

    template <typename T, unsigned N>
    Vector<T, N>& clampUpdate(Vector<T, N>& v, T min, T max)
    {
        for (auto i = 0u; i < N; ++i)
            clamp(v[i], min, max);
    }

    template <typename T, unsigned N>
    Vector<T, N> clamp(Vector<T, N> v, T min, T max)
    {
        clampUpdate(v, min, max);
        return v;
    }

    template <typename T>
    Vector<T, 2> reflect(const Vector<T, 2>& v,
                         const Vector<T, 2>& mirror)
    {
        auto n = getNormal(mirror);
        return v - 2 * (v * n) * n;
    }

    template <typename T>
    Vector<T, 2> rotate(const Vector<T, 2>& v, double radians)
    {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return makeVector2(T(v[0] * c - v[1] * s), T(v[0] * s + v[1] * c));
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
    bool isNull(Vector<T, N>& v, T = 0)
    {
        for (auto n: v)
        {
            if (n != 0)
                return false;
        }
        return true;
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool isNull(Vector<T, N>& v, T margin = Constants<T>::DEFAULT_MARGIN)
    {
        for (auto n: v)
        {
            if (fabs(n) > margin)
                return false;
        }
        return true;
    }

    using Vector2i = Vector<int, 2>;
    using Vector2f = Vector<float, 2>;
    using Vector2d = Vector<double, 2>;

    using Vector3i = Vector<int, 3>;
    using Vector3f = Vector<float, 3>;
    using Vector3d = Vector<double, 3>;

    using Vector4i = Vector<int, 4>;
    using Vector4f = Vector<float, 4>;
    using Vector4d = Vector<double, 4>;
}

namespace std
{
    template <typename T, size_t N>
    struct tuple_size<Xyz::Vector<T, N>>
        : integral_constant<size_t, N>
    {};

    template <size_t I, typename T, size_t N>
    struct tuple_element<I, Xyz::Vector<T, N>>
    {
        using type = T;
    };
}
