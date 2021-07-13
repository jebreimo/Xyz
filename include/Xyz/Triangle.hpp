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
    template <typename T>
    class Triangle
    {
    public:
        Triangle() = default;

        Triangle(const Vector<T, 2>& a,
                 const Vector<T, 2>& b,
                 const Vector<T, 2>& c)
            : points{a, b, c}
        {}

        Vector<T, 2> points[3];
    };

    template <typename T>
    Triangle<T> makeTriangle(const Vector<T, 2>& a,
                             const Vector<T, 2>& b,
                             const Vector<T, 2>& c)
    {
        return Triangle<T>(a, b, c);
    }

    template <typename T, typename U,
              typename Float = typename FloatType<decltype(T() + U())>::type>
    bool containsPoint(const Triangle<T>& triangle,
                       const Vector<U, 2>& point,
                       Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto a = dot(getNormal(triangle.points[1] - triangle.points[0]),
                     (point - triangle.points[0]));
        if (Approx<Float>(a, margin) <= 0)
            return false;
        auto b = dot(getNormal(triangle.points[2] - triangle.points[1]),
                     (point - triangle.points[1]));
        if (Approx<Float>(b, margin) <= 0)
            return false;
        auto c = dot(getNormal(triangle.points[0] - triangle.points[2]),
                     (point - triangle.points[2]));
        return Approx<Float>(c, margin) > 0;
    }

    template <typename T, typename U,
              typename Float = typename FloatType<decltype(T() + U())>::type>
    bool containsPointInclusive(const Triangle<T>& triangle,
                                const Vector<U, 2>& point,
                                Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto a = dot(getNormal(triangle.points[1] - triangle.points[0]),
                     (point - triangle.points[0]));
        if (Approx<Float>(a, margin) < 0)
            return false;
        auto b = dot(getNormal(triangle.points[2] - triangle.points[1]),
                     (point - triangle.points[1]));
        if (Approx<Float>(b, margin) < 0)
            return false;
        auto c = dot(getNormal(triangle.points[0] - triangle.points[2]),
                     (point - triangle.points[2]));
        return Approx<Float>(c, margin) >= 0;
    }
}
