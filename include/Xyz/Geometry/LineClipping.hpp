//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 24.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

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
    unsigned computeClippingOutcode(const Rectangle<T>& rectangle,
                                    const Vector<T, 2>& point)
    {
        auto bl = rectangle.bottomLeft();
        auto tr = rectangle.topRight();
        unsigned code = OUTCODE_INSIDE;

        if (get<0>(tr) < get<0>(point))
            code = OUTCODE_RIGHT;
        else if (get<0>(point) < get<0>(bl))
            code = OUTCODE_LEFT;

        if (get<1>(tr) < get<1>(point))
            code += OUTCODE_TOP;
        else if (get<1>(point) < get<1>(bl))
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
     *      which case both positions are -1.
     */
    template <typename T>
    std::pair<double, double> getClippingPositions(
            const Rectangle<T>& rectangle, const LineSegment<T, 2>& line)
    {
        auto startCode = computeClippingOutcode(rectangle, line.start());
        auto endCode = computeClippingOutcode(rectangle, line.end());
        auto tStart = 0.0, tEnd = 1.0;

        for (;;)
        {
            if (!(startCode | endCode))
                return {tStart, tEnd};
            if (startCode & endCode)
                return {-1.0, -1.0};
            auto start = line.start();
            auto vector = line.end() - line.start();
            auto bottomLeft = rectangle.bottomLeft();
            auto topRight = rectangle.topRight();

            unsigned code = startCode ? startCode : endCode;
            double t;
            if (code & OUTCODE_TOP)
                t = (get<1>(topRight) - get<1>(start)) / get<1>(vector);

            else if (code & OUTCODE_BOTTOM)
                t = (get<1>(bottomLeft) - get<1>(start)) / get<1>(vector);
            else if (code & OUTCODE_LEFT)
                t = (get<0>(bottomLeft) - get<0>(start)) / get<0>(vector);
            else
                t = (get<0>(topRight) - get<0>(start)) / get<0>(vector);

            auto point = start + T(t) * vector;
            if (code == startCode)
            {
                tStart = t;
                startCode = computeClippingOutcode(rectangle, point);
            }
            else
            {
                tEnd = t;
                endCode = computeClippingOutcode(rectangle, point);
            }
        }
    }
}
