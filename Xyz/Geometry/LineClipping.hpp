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

        if (x(tr) < x(point))
            code = OUTCODE_RIGHT;
        else if (x(point) < x(bl))
            code = OUTCODE_LEFT;

        if (y(tr) < y(point))
            code += OUTCODE_TOP;
        else if (y(point) < y(bl))
            code += OUTCODE_BOTTOM;

        return code;
    }

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
                t = (y(topRight) - y(start)) / y(vector);

            else if (code & OUTCODE_BOTTOM)
                t = (y(bottomLeft) - y(start)) / y(vector);
            else if (code & OUTCODE_LEFT)
                t = (x(bottomLeft) - x(start)) / x(vector);
            else
                t = (x(topRight) - x(start)) / x(vector);

            auto point = start + t * vector;
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
