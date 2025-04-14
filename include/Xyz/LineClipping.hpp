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
#include "LineSegment.hpp"
#include "Rectangle.hpp"

namespace Xyz
{
    constexpr unsigned OUTCODE_INSIDE = 0;
    constexpr unsigned OUTCODE_LEFT = 0b0001;
    constexpr unsigned OUTCODE_RIGHT = 0b0010;
    constexpr unsigned OUTCODE_BOTTOM = 0b0100;
    constexpr unsigned OUTCODE_TOP = 0b1000;

    template <typename T>
    [[nodiscard]]
    unsigned compute_clipping_outcode(const Rectangle<T>& rectangle,
                                      const Vector<T, 2>& point)
    {
        auto [x, y] = point;
        auto [x0, y0] = get_min(rectangle);
        auto [x1, y1] = get_max(rectangle);
        unsigned code = OUTCODE_INSIDE;

        if (x1 < x)
            code = OUTCODE_RIGHT;
        else if (x < x0)
            code = OUTCODE_LEFT;

        if (y1 < y)
            code += OUTCODE_TOP;
        else if (y < y0)
            code += OUTCODE_BOTTOM;

        return code;
    }

    /** @brief Returns the relative start and end positions of @a line
     *      inside @a rectangle.
     *
     * Uses the Cohen-Sutherland algorithm to compute the relative positions.
     *
     * @tparam T a numeric type.
     * @param rectangle the clipping rectangle.
     * @param line the line that will be clipped.
     * @return the relative start and end positions of the part of @a line
     *      that lies inside @a rectangle. Both positions will be between
     *      0 and 1, unless @a line is completely outside @a rectangle in
     *      which case the result is empty.
     */
    template <typename T>
    [[nodiscard]]
    std::optional<std::pair<double, double>>
    get_clipping_positions(const Rectangle<T>& rectangle,
                           const LineSegment<T, 2>& line)
    {
        auto start_code = compute_clipping_outcode(rectangle, line.start());
        auto end_code = compute_clipping_outcode(rectangle, line.end());
        double t_start = 0.0, tEnd = 1.0;

        for (;;)
        {
            if (!(start_code | end_code))
                return std::pair(t_start, tEnd);
            if (start_code & end_code)
                return std::nullopt;
            auto start = line.start();
            auto vector = line.end() - line.start();
            auto bottom_left = get_min(rectangle);
            auto top_right = get_max(rectangle);

            unsigned code = start_code ? start_code : end_code;
            double t;
            if (code & OUTCODE_TOP)
                t = (get<1>(top_right) - get<1>(start)) / get<1>(vector);
            else if (code & OUTCODE_BOTTOM)
                t = (get<1>(bottom_left) - get<1>(start)) / get<1>(vector);
            else if (code & OUTCODE_LEFT)
                t = (get<0>(bottom_left) - get<0>(start)) / get<0>(vector);
            else
                t = (get<0>(top_right) - get<0>(start)) / get<0>(vector);

            auto point = start + t * vector;
            if (code == start_code)
            {
                t_start = t;
                start_code = compute_clipping_outcode(rectangle, point);
            }
            else
            {
                tEnd = t;
                end_code = compute_clipping_outcode(rectangle, point);
            }
        }
    }
}
