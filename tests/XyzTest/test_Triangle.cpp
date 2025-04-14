//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 06.03.2016.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include "Ytest/Ytest.hpp"

namespace
{
    void test_contains_point()
    {
        auto triangle = Xyz::make_triangle(Xyz::make_vector2(-5, 0),
                                           Xyz::make_vector2(5, 0),
                                           Xyz::make_vector2(0, 5));
        Y_ASSERT(contains_point(triangle, Xyz::make_vector2(0, 2), 1e-12));
        Y_ASSERT(!contains_point(triangle, Xyz::make_vector2(-3, 2), 1e-12));
    }

    void test_contains_point_inclusive()
    {
        auto tri = Xyz::make_triangle(Xyz::make_vector2(-5, 0),
                                      Xyz::make_vector2(5, 0),
                                      Xyz::make_vector2(0, 5));
        Y_ASSERT(contains_point_inclusive(tri, Xyz::make_vector2(0, 2), 1e-12));
        Y_ASSERT(contains_point_inclusive(tri, Xyz::make_vector2(-3, 2), 1e-12));
    }

    void test_get_area()
    {
        auto tri = Xyz::make_triangle(Xyz::make_vector3(3, 1, 2),
                                      Xyz::make_vector3(7, 5, 2),
                                      Xyz::make_vector3(3, 7, 2));
        Y_EQUIVALENT(Xyz::get_area_squared(tri), 144, 1e-10);
        Y_EQUIVALENT(Xyz::get_area(tri), 12, 1e-10);
    }

    Y_SUBTEST("Geometry",
              test_contains_point,
              test_contains_point_inclusive,
              test_get_area);
}
