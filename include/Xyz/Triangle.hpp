//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 26.02.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Vector.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class Triangle
    {
    public:
        constexpr Triangle() = default;

        constexpr Triangle(const Vector<T, N>& a,
                           const Vector<T, N>& b,
                           const Vector<T, N>& c)
            : points{a, b, c}
        {}

        constexpr const Vector<T, N>& operator[](unsigned i) const
        {
            return points[i];
        }

        constexpr Vector<T, N>& operator[](unsigned i)
        {
            return points[i];
        }

        Vector<T, N> points[3];
    };

    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr Triangle<T, N> makeTriangle(const Vector<T, N>& a,
                                          const Vector<T, N>& b,
                                          const Vector<T, N>& c)
    {
        return Triangle<T, N>(a, b, c);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    auto getAreaSquared(const Triangle<T, N>& triangle)
    {
        // Heron's formula with minimal use of square roots.
        auto a2 = getLengthSquared(triangle[1] - triangle[0]);
        auto b2 = getLengthSquared(triangle[2] - triangle[1]);
        auto c2 = getLengthSquared(triangle[0] - triangle[2]);
        auto d = a2 + b2 - c2;
        return (4 * a2 * b2 - d * d) / 16;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    typename FloatType<T>::type getArea(const Triangle<T, N>& triangle)
    {
        return std::sqrt(getAreaSquared(triangle));
    }

    template <typename T, typename U,
              typename Float = typename FloatType<decltype(T() + U())>::type>
    [[nodiscard]]
    bool containsPoint(const Triangle<T, 2>& triangle,
                       const Vector<U, 2>& point,
                       Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto a = dot(getNormal(triangle[1] - triangle[0]),
                     (point - triangle[0]));
        if (Xyz::Approx<Float>(a, margin) <= 0)
            return false;
        auto b = dot(getNormal(triangle[2] - triangle[1]),
                     (point - triangle[1]));
        if (Xyz::Approx<Float>(b, margin) <= 0)
            return false;
        auto c = dot(getNormal(triangle[0] - triangle[2]),
                     (point - triangle[2]));
        return Xyz::Approx<Float>(c, margin) > 0;
    }

    template <typename T, typename U,
              typename Float = typename FloatType<decltype(T() + U())>::type>
    [[nodiscard]]
    bool containsPointInclusive(const Triangle<T, 2>& triangle,
                                const Vector<U, 2>& point,
                                Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto a = dot(getNormal(triangle[1] - triangle[0]),
                     (point - triangle[0]));
        if (Xyz::Approx<Float>(a, margin) < 0)
            return false;
        auto b = dot(getNormal(triangle[2] - triangle[1]),
                     (point - triangle[1]));
        if (Xyz::Approx<Float>(b, margin) < 0)
            return false;
        auto c = dot(getNormal(triangle[0] - triangle[2]),
                     (point - triangle[2]));
        return Xyz::Approx<Float>(c, margin) >= 0;
    }
}
