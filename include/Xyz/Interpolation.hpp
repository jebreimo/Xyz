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
    T bilinear(const Vector<T, 4>& q,
               const Vector<std::type_identity_t<T>, 2>& min,
               const Vector<std::type_identity_t<T>, 2>& max,
               const Vector<std::type_identity_t<T>, 2>& p)
    {
        auto [dx12, dy12] = max - min;
        auto [dx1, dy1] = p - min;
        auto [dx2, dy2] = max - p;
        return (q[0] * dx2 * dy2 + q[1] * dx1 * dy2
                + q[2] * dx2 * dy1 + q[3] * dx1 * dy1)
            / (dx12 * dy12);
    }
}
