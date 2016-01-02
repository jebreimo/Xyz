//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-01.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "MatrixClass.hpp"

namespace Xyz
{
    template <typename T>
    Matrix<T, 4> makePerspectiveProjection(const Vector<T, 3>& center,
                                           const Vector<T, 3>& size)
    {
        auto l = center[0] - size[0] * T(0.5);
        auto r = center[0] + size[0] * T(0.5);
        auto b = center[1] - size[1] * T(0.5);
        auto t = center[1] + size[1] * T(0.5);
        auto n = center[2] - size[2] * T(0.5);
        auto f = center[2] + size[2] * T(0.5);
        return Matrix<T, 4>{
                2 * n / (r - l),  0,  (r + l) / (r - l),  0,
                0,  2 * n / (t - b),  (t + b) / (t - b),  0,
                0,  0,  -(f + n) / (f - n),  -2 * f * n / (f - n),
                0,  0,  -1,  0};
    }

    template <typename T>
    Matrix<T, 4> makeOrthogonalProjection(const Vector<T, 3>& center,
                                          const Vector<T, 3>& size)
    {
        auto l = center[0] - size[0] * T(0.5);
        auto r = center[0] + size[0] * T(0.5);
        auto b = center[1] - size[1] * T(0.5);
        auto t = center[1] + size[1] * T(0.5);
        auto n = center[2] - size[2] * T(0.5);
        auto f = center[2] + size[2] * T(0.5);
        return Matrix<T, 4> {
                2 / (r - l),  0,  0,  -(r + l) / (r - l),
                0,  2 / (t - b),  0,  -(t + b) / (t - b),
                0,  0,  -2 / (f - n),  -(f + n) / (f - n),
                0, 0, 0, 1};
    }
}
