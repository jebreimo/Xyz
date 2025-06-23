//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "FloatType.hpp"
#include "Matrix.hpp"

namespace Xyz
{
    template <typename T>
    struct Plane
    {
        Vector<T, 3> origin;
        Vector<T, 3> normal;

        static constexpr Plane xy()
        {
            return {{0, 0, 0}, {0, 0, 1}};
        }

        static constexpr Plane xz()
        {
            return {{0, 0, 0}, {0, 1, 0}};
        }

        static constexpr Plane yz()
        {
            return {{0, 0, 0}, {1, 0, 0}};
        }
    };

    template <typename T, typename FloatT = FloatType_t<T>>
    [[nodiscard]]
    Matrix<FloatT, 4, 4> make_projection_matrix(const Plane<T>& plane)
    {
        const auto& n = vector_cast<FloatT>(plane.normal);
        const auto& p0 = vector_cast<FloatT>(plane.origin
        )
        ;
        auto nn = dot(n, n);

        auto result = Matrix<FloatT, 4, 4>::identity();

        // 3x3 part: I - nn^T / (n.n)
        for (unsigned i = 0; i < 3; ++i)
            for (unsigned j = 0; j < 3; ++j)
                result[{i, j}] -= n[i] * n[j] / nn;

        // translation part: (p0.n / n.n) * n
        auto d = dot(p0, n) / nn;
        Vector<FloatT, 3> t = n * d;

        // Fill 4x4 matrix
        for (unsigned i = 0; i < 3; ++i)
            result[{i, 3}] = t[i];

        return result;
    }
}
