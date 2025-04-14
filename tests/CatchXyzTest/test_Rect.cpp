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
    Xyz::RectangleI rect({10, 20}, {100, 80});
    REQUIRE(Xyz::get_center(rect) == Xyz::Vector2I(60, 60));
    REQUIRE(Xyz::get_min(rect) == Xyz::Vector2I(10, 20));
    REQUIRE(Xyz::get_bottom_left(rect) == Xyz::Vector2I(10, 20));
    REQUIRE(Xyz::get_bottom_right(rect) == Xyz::Vector2I(110, 20));
    REQUIRE(Xyz::get_top_left(rect) == Xyz::Vector2I(10, 100));
    REQUIRE(Xyz::get_top_right(rect) == Xyz::Vector2I(110, 100));
    REQUIRE(Xyz::get_max(rect) == Xyz::Vector2I(110, 100));

    Xyz::set_center(rect, Xyz::Vector2I(45, 75));
    REQUIRE(rect.origin == Xyz::Vector2I(-5, 35));
}

TEST_CASE("Test Rectangle min and max")
{
    auto get_min = [](const Xyz::Rectangle<int>& r){return Xyz::get_min(r);};
    auto get_max = [](const Xyz::Rectangle<int>& r){return Xyz::get_max(r);};

    REQUIRE(get_min({{10, 15}, {10, 10}}) == Xyz::Vector2I(10, 15));
    REQUIRE(get_max({{10, 15}, {10, 10}}) == Xyz::Vector2I(20, 25));
    REQUIRE(get_min({{10, 15}, {-10, 10}}) == Xyz::Vector2I(0, 15));
    REQUIRE(get_max({{10, 15}, {-10, 10}}) == Xyz::Vector2I(10, 25));
    REQUIRE(get_min({{10, 15}, {10, -10}}) == Xyz::Vector2I(10, 5));
    REQUIRE(get_max({{10, 15}, {10, -10}}) == Xyz::Vector2I(20, 15));
    REQUIRE(get_min({{10, 15}, {-10, -10}}) == Xyz::Vector2I(0, 5));
    REQUIRE(get_max({{10, 15}, {-10, -10}}) == Xyz::Vector2I(10, 15));
}

TEST_CASE("Test Rectangle is_empty")
{
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {0, 0})));
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {0, -1})));
    REQUIRE(is_empty(Xyz::RectangleF({2, 3}, {-1, 0})));
    REQUIRE(!is_empty(Xyz::RectangleF({2, 3}, {-1, -1})));
}

TEST_CASE("Test Rectangle normalize")
{
    using R = Xyz::RectangleD;
    REQUIRE(Xyz::normalize(R({10, 15}, {10, 10})) == R({10, 15}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {-10, 10})) == R({0, 15}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {10, -10})) == R({10, 5}, {10, 10}));
    REQUIRE(Xyz::normalize(R({10, 15}, {-10, -10})) == R({0, 5}, {10, 10}));
}
