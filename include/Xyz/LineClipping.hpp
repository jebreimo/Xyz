//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 24.04.2017.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <optional>
#include <utility>
#include "Vector.hpp"

namespace Xyz
{
    constexpr unsigned OUTCODE_INSIDE = 0;
    constexpr unsigned OUTCODE_LEFT = 0b0001;
    constexpr unsigned OUTCODE_RIGHT = 0b0010;
    constexpr unsigned OUTCODE_BOTTOM = 0b0100;
    constexpr unsigned OUTCODE_TOP = 0b1000;

    template <typename T>
    [[nodiscard]]
    unsigned compute_clipping_outcode(const Vector<T, 2>& point)
    {
        auto [x, y] = point;
        unsigned code = OUTCODE_INSIDE;

        if (1 < x)
            code = OUTCODE_RIGHT;
        else if (x < 0)
            code = OUTCODE_LEFT;

        if (1 < y)
            code += OUTCODE_TOP;
        else if (y < 0)
            code += OUTCODE_BOTTOM;

        return code;
    }

    /** @brief Returns the relative start and end positions of @a line
     *      inside the unit square.
     *
     * Uses the Cohen-Sutherland algorithm to compute the relative positions.
     *
     * @tparam T a numeric type.
     * @param start the start point of the line.
     * @param end the end point of the line.
     * @return the relative start and end positions of the part of the line
     *      that lies inside the unit square. Both positions will be between
     *      0 and 1, unless the line lies completely outside the unit square,
     *      in which case the result is empty.
     */
    template <typename T>
    [[nodiscard]]
    std::optional<std::pair<T, T>>
    get_clipping_positions(const Vector<T, 2>& start,
                           const Vector<T, 2>& end)
    {
        static_assert(std::is_floating_point_v<T>);
        auto start_code = compute_clipping_outcode(start);
        auto end_code = compute_clipping_outcode(end);
        T t_start = 0.0, t_end = 1.0;

        for (;;)
        {
            if (!(start_code | end_code))
                return std::pair(t_start, t_end);
            if (start_code & end_code)
                return std::nullopt;

            auto vector = end - start;

            unsigned code = start_code ? start_code : end_code;
            T t;
            if (code & OUTCODE_TOP)
                t = (1 - get<1>(start)) / get<1>(vector);
            else if (code & OUTCODE_BOTTOM)
                t = -get<1>(start) / get<1>(vector);
            else if (code & OUTCODE_LEFT)
                t = -get<0>(start) / get<0>(vector);
            else
                t = (1 - get<0>(start)) / get<0>(vector);

            auto point = start + t * vector;
            if (code == start_code)
            {
                t_start = t;
                start_code = compute_clipping_outcode(point);
            }
            else
            {
                t_end = t;
                end_code = compute_clipping_outcode(point);
            }
        }
    }
}
