//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-04-14.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief Performs bilinear interpolation of a value in a rectangle.
     *
     * @param q The values at the corners of the rectangle. The order is
     *  (0, 0), (1, 0), (0, 1), (1, 1).
     * @param p The point to interpolate.
     * @param min The minimum corner of the rectangle.
     * @param max The maximum corner of the rectangle.
     * @return The interpolated value.
     */
    template <typename T, typename U>
    T bilinear(const Vector<T, 4>& q,
               const Vector<U, 2>& p,
               const Vector<std::type_identity_t<U>, 2>& min,
               const Vector<std::type_identity_t<U>, 2>& max)
    {
        auto [dx12, dy12] = max - min;
        auto [dx1, dy1] = p - min;
        auto [dx2, dy2] = max - p;

        return static_cast<T>(
            (
                q[0] * dx2 * dy2
                + q[1] * dx1 * dy2
                + q[2] * dx2 * dy1
                + q[3] * dx1 * dy1
            ) / (dx12 * dy12));
    }

    /**
     * @brief Performs bilinear interpolation of a value in a rectangle.
     *
     * @param q The values at the corners of the rectangle. The order is
     *  (0, 0), (1, 0), (0, 1), (1, 1).
     * @param p The point to interpolate. Coordinates are in the range
     *  [0, 1].
     * @return The interpolated value.
     */
    template <typename T, typename U>
    T bilinear(const Vector<T, 4>& q,
               const Vector<U, 2>& p)
    {
        return bilinear(q, p, {0, 0}, {1, 1});
    }
}
