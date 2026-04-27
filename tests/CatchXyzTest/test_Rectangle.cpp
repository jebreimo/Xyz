//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-04-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Rectangle.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Rectangle: equality")
{
    Xyz::RectangleI a({1, 2}, {10, 20});
    Xyz::RectangleI b({1, 2}, {10, 20});
    Xyz::RectangleI c({1, 2}, {10, 21});
    Xyz::RectangleI d({2, 2}, {10, 20});
    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}

TEST_CASE("Rectangle: is_empty and is_null")
{
    REQUIRE(!is_empty(Xyz::RectangleI({0, 0}, {5, 5})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {0, 5})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {5, 0})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {0, 0})));

    REQUIRE(!is_null(Xyz::RectangleI({0, 0}, {5, 5})));
    REQUIRE(!is_null(Xyz::RectangleI({0, 0}, {0, 5})));
    REQUIRE(is_null(Xyz::RectangleI({0, 0}, {0, 0})));
}

TEST_CASE("Rectangle: get_min and get_max")
{
    Xyz::RectangleI pos({2, 3}, {10, 20});
    REQUIRE(get_min(pos) == Xyz::Vector2I(2, 3));
    REQUIRE(get_max(pos) == Xyz::Vector2I(12, 23));

    Xyz::RectangleI neg({2, 3}, {-10, -20});
    REQUIRE(get_min(neg) == Xyz::Vector2I(-8, -17));
    REQUIRE(get_max(neg) == Xyz::Vector2I(2, 3));
}

TEST_CASE("Rectangle: get_center and set_center")
{
    Xyz::RectangleI rect({0, 0}, {10, 20});
    REQUIRE(get_center(rect) == Xyz::Vector2I(5, 10));

    set_center(rect, Xyz::Vector2I(0, 0));
    REQUIRE(rect.origin == Xyz::Vector2I(-5, -10));
    REQUIRE(get_center(rect) == Xyz::Vector2I(0, 0));
}

TEST_CASE("Rectangle: normalize")
{
    using R = Xyz::RectangleI;
    REQUIRE(normalize(R({2, 3}, {10, 20})) == R({2, 3}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {-10, 20})) == R({-8, 3}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {10, -20})) == R({2, -17}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {-10, -20})) == R({-8, -17}, {10, 20}));
}