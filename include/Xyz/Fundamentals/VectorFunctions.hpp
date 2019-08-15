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
#include "Xyz/Utilities/Constants.hpp"
#include "Xyz/Utilities/FloatType.hpp"

namespace Xyz
{
    template <typename T>
    Vector<T, 2> getNormal(const Vector<T, 2>& v)
    {
        return makeVector2(-v[1], v[0]);
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

    template <typename T>
    Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
    {
        return makeVector3(a[1] * b[2] - a[2] * b[1],
                           a[2] * b[0] - a[0] * b[2],
                           a[0] * b[1] - a[1] * b[0]);
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
    T getLengthSquared(const Vector<T, N>& v)
    {
        return v * v;
    }

    template <typename T, unsigned N>
    bool areEquivalent(const Vector<T, N>& u, const Vector<T, N>& v,
                       typename FloatType<T>::type epsilon = 1e-12)
    {
        return getLengthSquared(u - v) <= epsilon * epsilon;
    }

    template <typename T, unsigned N>
    auto getLength(const Vector<T, N>& v)
    {
        return std::sqrt(getLengthSquared(v));
    }

    template <typename T, unsigned N>
    auto getCosAngle(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return (u * v) / std::sqrt(getLengthSquared(u) * getLengthSquared(v));
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
    auto getCounterclockwiseAngle(const Vector<T, 2>& u,
                                  const Vector<T, 2>& v)
    {
        auto angle = getAngle(u, v);
        if (getNormal(u) * v >= 0)
            return angle;
        else
            return 2 * PI_64 - angle;
    }

    template <typename T, unsigned N>
    Vector<typename FloatType<T>::type, N> getUnit(const Vector<T, N>& v)
    {
        return v / getLength(v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& resizeAssign(Vector<T, N>& v, T newLength)
    {
        return v *= (newLength / getLength(v));
    }

    template <typename T, unsigned N>
    Vector<T, N> resize(const Vector<T, N>& v, T newLength)
    {
        return v * (newLength / length(v));
    }

    template <typename T, unsigned N>
    Vector<T, N>& clampAssign(Vector<T, N>& v, T min, T max)
    {
        for (auto i = 0u; i < N; ++i)
            clamp(v[i], min, max);
    }

    template <typename T, unsigned N>
    Vector<T, N> clamp(Vector<T, N> v, T min, T max)
    {
        clampAssign(v, min, max);
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

    template <typename T, unsigned N>
    bool isNull(Vector<T, N>& v, double epsilon = 1e-12)
    {
        for (auto n: v)
        {
            if (fabs(n) > 1e-12)
                return false;
        }
        return true;
    }

    template <typename T, unsigned N>
    T& getX(Vector<T, N>& v)
    {
        return v[0];
    }

    template <typename T, unsigned N>
    T getX(const Vector<T, N>& v)
    {
        return v[0];
    }

    template <typename T, unsigned N>
    Vector<T, N>& setX(Vector<T, N>& v, T x)
    {
        v[0] = x;
        return v;
    }

    template <typename T, unsigned N>
    T& getY(Vector<T, N>& v)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        return v[1];
    }

    template <typename T, unsigned N>
    T getY(const Vector<T, N>& v)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        return v[1];
    }

    template <typename T, unsigned N>
    Vector<T, N>& setY(Vector<T, N>& v, T y)
    {
        static_assert(N > 1, "Vector doesn't have a y-coordinate.");
        v[1] = y;
        return v;
    }

    template <typename T, unsigned N>
    T& getZ(Vector<T, N>& v)
    {
        static_assert(N > 2, "Vectord oesn't have a z-coordinate.");
        return v[2];
    }

    template <typename T, unsigned N>
    T getZ(const Vector<T, N>& v)
    {
        static_assert(N > 2, "Vector doesn't have a z-coordinate.");
        return v[2];
    }

    template <typename T, unsigned N>
    Vector<T, N>& setZ(Vector<T, N>& v, T z)
    {
        static_assert(N > 2, "Vector doesn't have a z-coordinate.");
        v[2] = z;
        return v;
    }

    template <typename T, unsigned N>
    T& getW(Vector<T, N>& v)
    {
        static_assert(N > 3, "Vector doesn't have a w-coordinate.");
        return v[N - 1];
    }

    template <typename T, unsigned N>
    T getW(const Vector<T, N>& v)
    {
        static_assert(N > 3, "Vector doesn't have a w-coordinate.");
        return v[N - 1];
    }

    template <typename T, unsigned N>
    Vector<T, N>& setW(Vector<T, N>& v, T z)
    {
        static_assert(N > 3, "Vector doesn't have a w-coordinate.");
        v[N - 1] = z;
        return v;
    }

    template <typename T, unsigned N>
    Vector<T, N>& setXY(Vector<T, N>& v, T x, T y)
    {
        setX(v, x);
        return setY(v, y);
    }

    template <typename T, unsigned N>
    Vector<T, N>& setXYZ(Vector<T, N>& v, T x, T y, T z)
    {
        setX(v, x);
        setY(v, y);
        return setZ(v, z);
    }

    template <typename T, unsigned N>
    Vector<T, N>& setXYZW(Vector<T, N>& v, T x, T y, T z, T w)
    {
        setX(v, x);
        setY(v, y);
        setZ(v, z);
        return setW(v, z);
    }
}
