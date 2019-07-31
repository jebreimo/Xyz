//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 26.02.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Vector.hpp"

namespace Xyz {

    template <typename T>
    class Triangle
    {
    public:
        Triangle()
        {}

        Triangle(const Vector<T, 2>& a,
                 const Vector<T, 2>& b,
                 const Vector<T, 2>& c)
            : m_Points{{a, b, c}}
        {}

        const Vector<T, 2>& point(size_t index) const
        {
            return m_Points[index];
        }

    private:
        std::array<Vector<T, 2>, 3> m_Points;
    };

    template <typename T>
    Triangle<T> makeTriangle(const Vector<T, 2>& a,
                             const Vector<T, 2>& b,
                             const Vector<T, 2>& c)
    {
        return Triangle<T>(a, b, c);
    }

    template <typename T, typename U>
    bool containsPoint(const Triangle<T>& triangle,
                       const Vector<U, 2>& point, double epsilon)
    {
        auto a = getNormal(triangle.point(1) - triangle.point(0))
                 * (point - triangle.point(0));
        if (lessOrEqual<T>(a, 0, epsilon))
            return false;
        auto b = getNormal(triangle.point(2) - triangle.point(1))
                 * (point - triangle.point(1));
        if (lessOrEqual<T>(b, 0, epsilon))
            return false;
        auto c = getNormal(triangle.point(0) - triangle.point(2))
                 * (point - triangle.point(2));
        return greater<T>(c, 0, epsilon);
    }

    template <typename T, typename U>
    bool containsPointInclusive(const Triangle<T>& triangle,
                                const Vector<U, 2>& point, double epsilon)
    {
        auto a = getNormal(triangle.point(1) - triangle.point(0))
                 * (point - triangle.point(0));
        if (less<T>(a, 0, epsilon))
            return false;
        auto b = getNormal(triangle.point(2) - triangle.point(1))
                 * (point - triangle.point(1));
        if (less<T>(b, 0, epsilon))
            return false;
        auto c = getNormal(triangle.point(0) - triangle.point(2))
                 * (point - triangle.point(2));
        return greaterOrEqual<T>(c, 0, epsilon);
    }
}
