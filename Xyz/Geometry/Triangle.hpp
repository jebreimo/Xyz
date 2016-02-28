//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 26.02.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "../Vector.hpp"

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
            : m_Points{a, b, c}
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
}
