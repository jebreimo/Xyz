//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief A pentagon defined by five points.
     *
     * The points are expected to be in counterclockwise order. Functions
     * that treat the pentagon as a surface assume that it is planar and
     * convex.
     */
    template <typename T, unsigned N>
    class Pentagon
    {
    public:
        constexpr Pentagon() = default;

        constexpr Pentagon(const Vector<T, N>& a,
                           const Vector<T, N>& b,
                           const Vector<T, N>& c,
                           const Vector<T, N>& d,
                           const Vector<T, N>& e)
            : points{a, b, c, d, e}
        {
        }

        constexpr const Vector<T, N>& operator[](unsigned i) const
        {
            return points[i];
        }

        constexpr Vector<T, N>& operator[](unsigned i)
        {
            return points[i];
        }

        Vector<T, N> points[5] = {};
    };

    template <typename T>
    [[nodiscard]]
    auto get_normal(const Pentagon<T, 3>& pentagon)
    {
        return normalize(cross(pentagon.points[1] - pentagon.points[0],
                               pentagon.points[2] - pentagon.points[1]));
    }
}
