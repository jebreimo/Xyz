//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Matrix.hpp"

namespace Xyz
{
    /**
     * Brief description of the transformation:
     * The origin is moved to the eye position. The negative
     * z-axis is aligned with the forward vector (center - eye). The
     * positive y-axis is aligned with the up vector as closely as
     * possible.
     */
    template <typename T>
    Matrix<T, 4, 4> make_look_at_matrix(const Vector<T, 3>& eye,
                                        const Vector<T, 3>& center,
                                        const Vector<T, 3>& up)
    {
        auto f = normalize(center - eye);
        auto s = cross(f, normalize(up));
        auto u = cross(s, f);
        return Matrix<T, 4, 4>{
                s[0], s[1], s[2], dot(-s, eye),
                u[0], u[1], u[2], dot(-u, eye),
                -f[0], -f[1], -f[2], dot(f, eye),
                0, 0, 0, 1};
    }

    /**
     * @brief Creates a perspective projection matrix.
     *
     * @note While the look_at matrix produces negative z coordinates,
     * the frustum expects positive values for the near and far clipping
     * planes. You have to pretend that the camera is looking in the
     * positive z direction when setting up the frustum.
     *
     * @param l The coordinate for the left vertical clipping plane.
     * @param r The coordinate for the right vertical clipping plane.
     * @param b The coordinate for the bottom horizontal clipping plane.
     * @param t The coordinate for the top horizontal clipping plane.
     * @param n The distance to the near clipping plane. Must be positive.
     * @param f The distance to the far clipping plane. Must be positive.
     * @return The perspective projection matrix.
     */
    template <typename T>
    Matrix<T, 4, 4> make_frustum_matrix(T l, T r, T b, T t, T n, T f)
    {
        return Matrix<T, 4, 4>{
                2 * n / (r - l),  0,  (r + l) / (r - l),  0,
                0,  2 * n / (t - b),  (t + b) / (t - b),  0,
                0,  0,  -(f + n) / (f - n),  -2 * f * n / (f - n),
                0,  0,  -1,  0};
    }

    template <typename T>
    Matrix<T, 4, 4> make_orthographic_matrix(T l, T r, T b, T t, T n, T f)
    {
        return Matrix<T, 4, 4> {
                2 / (r - l),  0,  0,  -(r + l) / (r - l),
                0,  2 / (t - b),  0,  -(t + b) / (t - b),
                0,  0,  -2 / (f - n),  -(f + n) / (f - n),
                0, 0, 0, 1};
    }
}
