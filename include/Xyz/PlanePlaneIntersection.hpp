//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <optional>
#include "Line.hpp"
#include "Plane.hpp"

namespace Xyz
{
    template <typename T, typename FloatT = FloatType_t<T>>
    [[nodiscard]]
    std::optional<Line<FloatT, 3>>
    get_intersection(const Plane<T>& p1, const Plane<T>& p2,
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
                point[0] = (s - p1.normal[1] * point[1]) / p1.normal[0];
            }
            else
            {
                point[1] = 0;
                point[2] = dividend / -vec[1];
                point[0] = (s - p1.normal[2] * point[2]) / p1.normal[0];
            }
        }
        else if (std::abs(p1.normal[1]) > margin)
        {
            FloatT dividend = p1.normal[1] * t - p2.normal[1] * s;
            if (std::abs(vec[2]) > margin)
            {
                point[2] = 0;
                point[0] = dividend / -vec[2];
                point[1] = (s - p1.normal[0] * point[0]) / p1.normal[1];
            }
            else
            {
                point[0] = 0;
                point[2] = dividend / vec[0];
                point[1] = (s - p1.normal[2] * point[2]) / p1.normal[1];
            }
        }
        else
        {
            FloatT dividend = p1.normal[2] * t - p2.normal[2] * s;
            if (std::abs(vec[1]) > margin)
            {
                point[1] = 0;
                point[0] = dividend / vec[1];
                point[2] = (s - p1.normal[0] * point[0]) / p1.normal[2];
            }
            else
            {
                point[0] = 0;
                point[1] = dividend / -vec[0];
                point[2] = (s - p1.normal[1] * point[1]) / p1.normal[2];
            }
        }
        return {Line(point, vec)};
    }
}
