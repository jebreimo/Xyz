//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 13.02.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include "Ytest/Ytest.hpp"

namespace {
    using namespace Xyz;

    template <typename T>
    Xyz::LineSegment<T, 2> make_line_segment(T x1, T y1, T x2, T y2)
    {
        return Xyz::make_line_segment(make_vector2(x1, y1), make_vector2(x2, y2));
    }

    static void test_get_intersection_factors()
    {
        auto a = make_line_segment<int>(-4, -2, 4, 2);
        auto b = make_line_segment<int>(3, -9, -1, 3);
        auto intersection = get_intersection_positions(a, b, 1e-10);
        Y_EQUAL(std::get<0>(intersection), LineRelationship::INTERSECTING);
        Y_EQUAL(std::get<1>(intersection), 0.5);
        Y_EQUAL(std::get<2>(intersection), 0.75);
    }

    Y_SUBTEST("Geometry",
              test_get_intersection_factors
    );

}
