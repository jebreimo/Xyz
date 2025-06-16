//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <optional>
#include "Approx.hpp"
#include "Line.hpp"
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

    template <typename T, typename FloatT = FloatType_t<T>>
    [[nodiscard]]
    std::optional<Line<FloatT, 3>>
    get_intersection(const Plane<T, 3>& p1, const Plane<T, 3>& p2,
                     std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        auto vec = cross(p1.normal, p2.normal);
        if (are_equal(vec, Vector<T, 3>(), margin))
            return std::nullopt; // Planes are parallel

        Vector<T, 3> point;
        auto s = dot(p1.origin, p1.normal);
        auto t = dot(p2.origin, p2.normal);
        if (std::abs(p1.normal[0]) > margin)
        {
            FloatT dividend = p1.normal[0] * t - p2.normal[0] * s;
            if (std::abs(vec[2]) > margin)
            {
                point[2] = 0;
                point[1] = dividend / vec[2];
                point[0] = s - p1.normal[1] * point[1] / p1.normal[0];
            }
            else
            {
                point[1] = 0;
                point[2] = dividend / -vec[1];
                point[0] = s - p1.normal[2] * point[2] / p1.normal[0];
            }
        }
        else if (std::abs(p1.normal[1]) > margin)
        {
            FloatT dividend = p1.normal[1] * t - p2.normal[1] * s;
            if (std::abs(vec[2]) > margin)
            {
                point[2] = 0;
                point[0] = dividend / -vec[2];
                point[1] = s - p1.normal[0] * point[0] / p1.normal[1];
            }
            else
            {
                point[0] = 0;
                point[2] = dividend / vec[0];
                point[1] = s - p1.normal[2] * point[2] / p1.normal[1];
            }
        }
        else
        {
            FloatT dividend = p1.normal[2] * t - p2.normal[2] * s;
            if (std::abs(vec[1]) > margin)
            {
                point[1] = 0;
                point[0] = dividend / vec[1];
                point[2] = s - p1.normal[0] * point[0] / p1.normal[2];
            }
            else
            {
                point[0] = 0;
                point[1] = dividend / -vec[0];
                point[2] = s - p1.normal[1] * point[1] / p1.normal[2];
            }
        }
        return {Line(point, vec)};
    }
}
