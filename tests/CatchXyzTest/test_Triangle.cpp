//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 06.03.2016.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("test contains point")
{
    auto tri = Xyz::make_triangle(Xyz::make_vector2(-5, 0),
                                  Xyz::make_vector2(5, 0),
                                  Xyz::make_vector2(0, 5));
    REQUIRE(contains_point(tri, Xyz::make_vector2(0, 2), 1e-12));
    REQUIRE(!contains_point(tri, Xyz::make_vector2(-3, 2), 1e-12));
}

TEST_CASE("test contains point inclusive")
{
    auto tri = Xyz::make_triangle(Xyz::make_vector2(-5, 0),
                                  Xyz::make_vector2(5, 0),
                                  Xyz::make_vector2(0, 5));
    REQUIRE(contains_point_inclusive(tri, Xyz::make_vector2(0, 2), 1e-12));
    REQUIRE(contains_point_inclusive(tri, Xyz::make_vector2(-3, 2), 1e-12));
}

TEST_CASE("test get area")
{
    using Catch::Matchers::WithinAbs;
    auto tri = Xyz::make_triangle(Xyz::make_vector3(3, 1, 2),
                                  Xyz::make_vector3(7, 5, 2),
                                  Xyz::make_vector3(3, 7, 2));
    REQUIRE_THAT(Xyz::get_area_squared(tri), WithinAbs(144, 1e-10));
    REQUIRE_THAT(Xyz::get_area(tri), WithinAbs(12, 1e-10));
}
