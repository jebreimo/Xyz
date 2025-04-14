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
    template <typename T>
    T bilinear(const Vector<T, 2>& p,
               const Vector<T, 2>& p1,
               const Vector<T, 2>& p2,
               T q11, T q21, T q12, T q22)
    {
        auto [dx12, dy12] = p2 - p1;
        auto [dx1, dy1] = p - p1;
        auto [dx2, dy2] = p2 - p;
        return (q11 * dx2 * dy2 + q21 * dx1 * dy2
                + q12 * dx2 * dy1 + q22 * dx1 * dy1)
            / (dx12 * dy12);
    }
}
