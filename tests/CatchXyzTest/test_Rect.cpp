//****************************************************************************
// Copyright © 2023 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2023-08-19.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Rectangle.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test Rectangle")
{
    Xyz::RectangleD rect({10, 20}, {100, 80});
    REQUIRE(Xyz::get_center(rect) == Xyz::Vector2D(60, 60));
    REQUIRE(rect[0] == Xyz::Vector2I(10, 20));
    REQUIRE(rect[1] == Xyz::Vector2I(110, 20));
    REQUIRE(rect[3] == Xyz::Vector2I(10, 100));
    REQUIRE(rect[2] == Xyz::Vector2I(110, 100));

    Xyz::set_center(rect, Xyz::Vector2D(45, 75));
    REQUIRE(rect.origin == Xyz::Vector2I(-5, 35));
}

TEST_CASE("Test Rectangle is_empty")
{
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {0, 0})));
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {0, -1})));
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {-1, 0})));
    REQUIRE(!is_empty(Xyz::RectangleF({2, 3}, {-1, -1})));
}

TEST_CASE("Test Rectangle: normalize without angle")
{
    using R = Xyz::RectangleD;
    constexpr auto pi = Xyz::Constants<double>::PI;
    REQUIRE(Xyz::normalize(R({10, 15}, {10, 10})) == R({10, 15}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {-10, 10})) == R({0, 15}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {10, -10})) == R({10, 5}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {-10, -10})) == R({0, 5}, {10, 10}));
}

TEST_CASE("Test Rectangle: normalize with angle")
{
    using R = Xyz::RectangleD;
    using V = Xyz::Vector2D;
    constexpr auto pi = Xyz::Constants<double>::PI;
    const auto rect = R({10, 15}, {10, 10}, pi);
    const auto result = Xyz::normalize(rect);
    REQUIRE(Xyz::are_equal(result.origin, V(0, 5)));
    REQUIRE(Xyz::are_equal(result.size, rect.size));
    REQUIRE(result.angle == 0);
}
