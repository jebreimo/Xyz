//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Matrix.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    struct Plane
    {
        Vector<T, N> origin;
        Vector<T, N> normal;
    };

    template <typename T>
    [[nodiscard]]
    Matrix<T, 4, 4> make_projection_matrix(const Plane<T, 3>& plane)
    {
        const auto& n = plane.normal;
        const auto& p0 = plane.origin;
        auto nn = dot(n, n);

        Matrix<T, 4, 4> result;

        // 3x3 part: I - nn^T / (n.n)
        for (unsigned i = 0; i < 3; ++i)
            for (unsigned j = 0; j < 3; ++j)
                result[{i, j}] = (i == j ? 1 : 0) - n[i] * n[j] / nn;

        // translation part: (p0.n / n.n) * n
        auto d = dot(p0, n) / nn;
        Vector<T, 3> t = n * d;

        // Fill 4x4 matrix
        for (unsigned i = 0; i < 3; ++i)
            result[{i, 3}] = t[i];

        result[{3, 3}] = 1;

        return result;
    }
}
