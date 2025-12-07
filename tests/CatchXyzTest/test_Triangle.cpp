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
    const auto tri = Xyz::Triangle(Xyz::Vector2I(-5, 0),
                                   Xyz::Vector2I(5, 0),
                                   Xyz::Vector2I(0, 5));
    REQUIRE(contains_point(tri, Xyz::Vector2I(0, 2), 1e-12));
    REQUIRE(!contains_point(tri, Xyz::Vector2I(-3, 2), 1e-12));
}

TEST_CASE("test contains point inclusive")
{
    const auto tri = Xyz::Triangle(Xyz::Vector2I(-5, 0),
                                   Xyz::Vector2I(5, 0),
                                   Xyz::Vector2I(0, 5));
    REQUIRE(contains_point_inclusive(tri, Xyz::Vector2I(0, 2), 1e-12));
    REQUIRE(contains_point_inclusive(tri, Xyz::Vector2I(-3, 2), 1e-12));
}

TEST_CASE("test get area")
{
    using Catch::Matchers::WithinAbs;
    const auto tri = Xyz::Triangle(Xyz::Vector3I(3, 1, 2),
                                   Xyz::Vector3I(7, 5, 2),
                                   Xyz::Vector3I(3, 7, 2));
    REQUIRE_THAT(Xyz::get_area_squared(tri), WithinAbs(144, 1e-10));
    REQUIRE_THAT(Xyz::get_area(tri), WithinAbs(12, 1e-10));
}

TEST_CASE("test get normal")
{
    const auto tri = Xyz::Triangle(Xyz::Vector3D(1, 0, 0),
                                   Xyz::Vector3D(0, 1, 0),
                                   Xyz::Vector3D(0, 0, 1));
    const auto normal = Xyz::get_normal(tri);
    REQUIRE(Xyz::are_equal(normal, Xyz::Vector3D(1, 1, 1)));

    auto tri2 = Xyz::Triangle(Xyz::Vector3D(0, 0, 0),
                                    Xyz::Vector3D(1, 2, 0),
                                    Xyz::Vector3D(2, 0, 0));
    const auto normal2 = Xyz::get_normal(tri2);
    REQUIRE(Xyz::are_equal(normal2, Xyz::Vector3D(0, 0, -4)));

    std::swap(tri2[1], tri2[2]);
    const auto normal3 = Xyz::get_normal(tri2);
    REQUIRE(Xyz::are_equal(normal3, Xyz::Vector3D(0, 0, 4)));
}
