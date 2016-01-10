//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-01.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "MatrixClass.hpp"
#include "VectorFunctions.hpp"

namespace Xyz
{
    template <typename T>
    Matrix<T, 4> makeLookAt(const Vector<T, 3>& cameraPos,
                            const Vector<T, 3>& centerPos,
                            const Vector<T, 3>& up)
    {
        auto f = getUnit(centerPos - cameraPos);
        auto s = cross(f, getUnit(up));
        auto u = cross(s, f);
        return Matrix<T, 4>{
                s[0], s[1], s[2], -s * cameraPos,
                u[0], u[1], u[2], -u * cameraPos,
                -f[0], -f[1], -f[2], f * cameraPos,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> makeFrustum(T l, T r, T b, T t, T n, T f)
    {
        return Matrix<T, 4>{
                2 * n / (r - l),  0,  (r + l) / (r - l),  0,
                0,  2 * n / (t - b),  (t + b) / (t - b),  0,
                0,  0,  -(f + n) / (f - n),  -2 * f * n / (f - n),
                0,  0,  -1,  0};
    }

    template <typename T>
    Matrix<T, 4> makeOrthogonal(T l, T r, T b, T t, T n, T f)
    {
        return Matrix<T, 4> {
                2 / (r - l),  0,  0,  -(r + l) / (r - l),
                0,  2 / (t - b),  0,  -(t + b) / (t - b),
                0,  0,  -2 / (f - n),  -(f + n) / (f - n),
                0, 0, 0, 1};
    }
}
