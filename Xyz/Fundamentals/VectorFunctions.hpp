//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include "VectorClass.hpp"
#include "../Utilities/Constants.hpp"

namespace Xyz
{
    template <typename T>
    Vector<T, 2> normal(const Vector<T, 2>& v)
    {
        return makeVector(-v[1], v[0]);
    }

    template<typename T, unsigned N>
    Vector<T, N>& assignDiv(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= v[i];
        return u;
    }

    template<typename T, unsigned N>
    Vector<T, N>& assignMul(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= v[i];
        return u;
    }

    template<typename T>
    Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
    {
        return makeVector(a[1] * b[2] - a[2] * b[1],
                          a[2] * b[0] - a[0] * b[2],
                          a[0] * b[1] - a[1] * b[0]);
    }

    template<typename T, unsigned N>
    Vector<T, N> div(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / v[i];
        return w;
    }

    template<typename T, typename U, unsigned N>
    Vector<T, N> div(U scalar, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = scalar / v[i];
        return w;
    }

    template<typename T, unsigned N>
    Vector<T, N> mul(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * v[i];
        return w;
    }

    template<typename T, unsigned N>
    T lengthSquared(const Vector<T, N>& v)
    {
        return v * v;
    }

    template<typename T, unsigned N>
    bool equivalent(const Vector<T, N>& u, const Vector<T, N>& v,
                    double epsilon = 1e-12)
    {
        return lengthSquared(u - v) <= epsilon * epsilon;
    }

    template <unsigned N>
    bool equivalent(const Vector<float, N>& u, const Vector<float, N>& v,
                    float epsilon = 1e-12)
    {
        return lengthSquared(u - v) <= epsilon * epsilon;
    }

    template<typename T, unsigned N>
    double length(const Vector<T, N>& v)
    {
        return std::sqrt(lengthSquared(v));
    }

    template <unsigned N>
    float length(const Vector<float, N>& v)
    {
        return std::sqrt(lengthSquared(v));
    }

    template<typename T, unsigned N>
    double cosAngle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return (u * v) / std::sqrt(lengthSquared(u) * lengthSquared(v));
    }

    template <unsigned N>
    float cosAngle(const Vector<float, N>& u, const Vector<float, N>& v)
    {
        return (u * v) / std::sqrt(lengthSquared(u) * lengthSquared(v));
    }

    /** @brief Returns the smallest angle between @a u and @a v.
      * @return A value in the range 0 <= angle <= pi.
      */
    template<typename T, unsigned N>
    double angle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return std::acos(cosAngle(u, v));
    }

    /** @brief Returns the smallest angle between @a u and @a v.
      * @return A value in the range 0 <= angle <= pi.
      */
    template <unsigned N>
    float angle(const Vector<float, N>& u, const Vector<float, N>& v)
    {
        return std::acos(cosAngle(u, v));
    }

    template <typename T>
    double counterclockwiseAngle(const Vector<T, 2>& u,
                                 const Vector<T, 2>& v)
    {
        auto angle = Xyz::angle(u, v);
        if (normal(u) * v >= 0)
            return angle;
        else
            return 2 * Pi - angle;
    }

    template<typename T, unsigned N>
    Vector<double, N> unit(const Vector<T, N>& v)
    {
        return v / length(v);
    }

    template <unsigned N>
    Vector<float, N> unit(const Vector<float, N>& v)
    {
        return v / length(v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& resize(Vector<T, N>& v, T newLength)
    {
        return v *= (newLength / length(v));
    }

    template<typename T, unsigned N>
    Vector<T, N> resized(const Vector<T, N>& v, T newLength)
    {
        return v * (newLength / length(v));
    }

    template<typename T, unsigned N>
    Vector<T, N>& clamp(Vector<T, N>& v, T min, T max)
    {
        for (auto i = 0u; i < N; ++i)
            clamp(v[i], min, max);
    }

    template<typename T, unsigned N>
    Vector<T, N> clamped(Vector<T, N> v, T min, T max)
    {
        clamp(v, min, max);
        return v;
    }

    template<typename T>
    Vector<double, 2> rotated(const Vector<T, 2>& v, double radians)
    {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return makeVector(v[0] * c - v[1] * s, v[0] * s + v[1] * c);
    }

    inline Vector<float, 2> rotated(const Vector<float, 2>& v, float radians)
    {
        auto c = std::cos(radians);
        auto s = std::sin(radians);
        return makeVector(v[0] * c - v[1] * s, v[0] * s + v[1] * c);
    }

    template<typename T, unsigned N>
    bool isNull(Vector<T, N>& v, double epsilon = 1e-12)
    {
        for (auto n: v)
        {
            if (fabs(n) > 1e-12)
                return false;
        }
        return true;
    }

    template<typename T, unsigned N>
    T& x(Vector<T, N>& v)
    {
        return v[0];
    }

    template<typename T, unsigned N>
    T x(const Vector<T, N>& v)
    {
        return v[0];
    }

    template<typename T, unsigned N>
    Vector<T, N>& setX(Vector<T, N>& v, T x)
    {
        v[0] = x;
        return v;
    }

    template<typename T, unsigned N>
    T& y(Vector<T, N>& v)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        return v[1];
    }

    template<typename T, unsigned N>
    T y(const Vector<T, N>& v)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        return v[1];
    }

    template<typename T, unsigned N>
    Vector<T, N>& setY(Vector<T, N>& v, T y)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        v[1] = y;
        return v;
    }

    template<typename T, unsigned N>
    T& z(Vector<T, N>& v)
    {
        static_assert(N > 2, "Vectord oesn't have a z-coordinate.");
        return v[2];
    }

    template<typename T, unsigned N>
    T z(const Vector<T, N>& v)
    {
        static_assert(N > 2, "Vector doesn't have a z-coordinate.");
        return v[2];
    }

    template<typename T, unsigned N>
    Vector<T, N>& setZ(Vector<T, N>& v, T z)
    {
        static_assert(N > 2, "Vector doesn't have a z-coordinate.");
        v[2] = z;
        return v;
    }

    template<typename T, unsigned N>
    T& w(Vector<T, N>& v)
    {
        return v[N - 1];
    }

    template<typename T, unsigned N>
    T w(const Vector<T, N>& v)
    {
        return v[N - 1];
    }

    template<typename T, unsigned N>
    Vector<T, N>& setW(Vector<T, N>& v, T z)
    {
        v[N - 1] = z;
        return v;
    }

    template<typename T, unsigned N>
    Vector<T, N>& setXY(Vector<T, N>& v, T x, T y)
    {
        setX(v, x);
        return setY(v, y);
    }

    template<typename T, unsigned N>
    Vector<T, N>& setXYZ(Vector<T, N>& v, T x, T y, T z)
    {
        setX(v, x);
        setY(v, y);
        return setZ(v, z);
    }

    template<typename T, unsigned N>
    Vector<T, N>& setXYZW(Vector<T, N>& v, T x, T y, T z, T w)
    {
        setX(v, x);
        setY(v, y);
        setZ(v, z);
        return setW(v, z);
    }
}
