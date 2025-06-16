//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Plane.hpp>

#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Plane: projection matrix on")
{
    using namespace Xyz;
    using Catch::Matchers::WithinAbs;

    constexpr Plane<double, 3> plane{{2, -1, 0}, {0, 1, 1}};
    const auto m = make_projection_matrix(plane);
    using V = Vector4D;
    CHECK(are_equal(m * V(2, -1, 0, 1), V(2, -1, 0, 1), 1e-10));
    CHECK(are_equal(m * V(5, 2, 3, 1), V(5, -1, 0, 1), 1e-10));
}

TEST_CASE("Plane: intersection with diagonal plane")
{
    using V = Xyz::Vector3F;
    using Plane = Xyz::Plane<float, 3>;
    Plane plane{{-1.5, 2, 5.5}, {-1, -1, 1}};
    SECTION("Intersection with x = 0 plane")
    {
        constexpr Plane x_is_0 {{0, 0, 0}, {1, 0, 0}};
        auto line = Xyz::get_intersection(x_is_0, plane);
        REQUIRE(line);
        REQUIRE(are_equal(line->point, V(0, -5, 0)));
        REQUIRE(are_equal(line->vector, V(0, -1, -1)));
    }
    SECTION("Intersection with y = 0 plane")
    {
        constexpr Plane y_is_0 {{0, 0, 0}, {0, 1, 0}};
        auto line = Xyz::get_intersection(y_is_0, plane);
        REQUIRE(line);
        REQUIRE(are_equal(line->point, V(-5, 0, 0)));
        REQUIRE(are_equal(line->vector, V(1, 0, 1)));
    }
    SECTION("Intersection with z = 0 plane")
    {
        constexpr Plane z_is_0 {{0, 0, 0}, {0, 0, 1}};
        auto line = Xyz::get_intersection(z_is_0, plane);
        REQUIRE(line);
        REQUIRE(are_equal(line->point, V(-5, 0, 0)));
        REQUIRE(are_equal(line->vector, V(1, -1, 0)));
    }
}
