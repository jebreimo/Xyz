//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 24.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Xyz.hpp"
#include "../../Submodules/Ytest/Ytest.hpp"

namespace
{
    void testClip(const Xyz::Rectangle<double>& rectangle,
                  const Xyz::Vector2d& start,
                  const Xyz::Vector2d& end,
                  double expectedT0, double expectedT1)
    {
        Xyz::LineSegment<double, 2> lineSegment(start, end);
        auto ts = Xyz::getClippingPositions(rectangle, lineSegment);
        Y_EQUIVALENT(ts.first, expectedT0, 1e-12);
        Y_EQUIVALENT(ts.second, expectedT1, 1e-12);
    }

    void test_clipping()
    {
        Xyz::Rectangle<double> rect({-10, -10}, {20, 20});
        Y_CALL(testClip(rect, {-5, -5}, {5, 5}, 0, 1));
        Y_CALL(testClip(rect, {-22, 0}, {0, 22}, -1, -1));
        Y_CALL(testClip(rect, {-20, -10}, {10, 20}, 1 / 3.0, 2 / 3.0));
        Y_CALL(testClip(rect, {10, 20}, {-20, -10}, 1 / 3.0, 2 / 3.0));
        Y_CALL(testClip(rect, {0, -8}, {0, 16}, 0, 0.75));
        Y_CALL(testClip(rect, {0, 16}, {0, -8}, 0.25, 1.0));
    }

    Y_SUBTEST("Geometry", test_clipping);
}
