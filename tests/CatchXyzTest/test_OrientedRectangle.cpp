//****************************************************************************
// Copyright © 2023 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2023-08-19.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/OrientedRectangle.hpp>
#include <catch2/catch_test_macros.hpp>

constexpr auto PI = Xyz::Constants<double>::PI;

TEST_CASE("OrientedRectangle: Test 2D rectangle")
{
    Xyz::OrientedRectangle2D rect({{10, 20}}, {100, 80});
    REQUIRE(Xyz::get_center(rect) == Xyz::Vector2D(60, 60));
    const auto o = rect.placement.origin;
    const auto [x, y] = Xyz::get_vectors(rect);
    REQUIRE(o == Xyz::Vector2I(10, 20));
    REQUIRE(o + x == Xyz::Vector2I(110, 20));
    REQUIRE(o + y == Xyz::Vector2I(10, 100));
    REQUIRE(o + x + y == Xyz::Vector2I(110, 100));

    Xyz::set_center(rect, Xyz::Vector2D(45, 75));
    REQUIRE(rect.placement.origin == Xyz::Vector2I(-5, 35));
}

TEST_CASE("OrientedRectangle: Test 2D rectangle is_empty")
{
    REQUIRE(is_empty(Xyz::OrientedRectangle2F({{2, 3}}, {0, 0})));
    REQUIRE(is_empty(Xyz::OrientedRectangle2F({{2, 3}}, {0, -1})));
    REQUIRE(is_empty(Xyz::OrientedRectangle2F({{2, 3}}, {-1, 0})));
    REQUIRE(!is_empty(Xyz::OrientedRectangle2F({{2, 3}}, {-1, -1})));
}

TEST_CASE("OrientedRectangle: Test 2D rectangle: normalize without angle")
{
    using R = Xyz::OrientedRectangle2D;
    REQUIRE(Xyz::normalize(R({{10, 15}}, {10, 10})) == R({{10, 15}}, {10, 10}));
    REQUIRE(Xyz::normalize(R({{10, 15}}, {-10, 10})) == R({{0, 15}}, {10, 10}));
    REQUIRE(Xyz::normalize(R({{10, 15}}, {10, -10})) == R({{10, 5}}, {10, 10}));
    REQUIRE(Xyz::normalize(R({{10, 15}}, {-10, -10})) == R({{0, 5}}, {10, 10}));
}

TEST_CASE("OrientedRectangle: Test 2D rectangle: normalize with angle")
{
    using R = Xyz::OrientedRectangle2D;
    using V = Xyz::Vector2D;
    constexpr auto pi = Xyz::Constants<double>::PI;
    const auto rect = R({{10, 15}, {pi}}, {10, 10});
    const auto result = Xyz::normalize(rect);
    REQUIRE(Xyz::are_equal(result.placement.origin, V(0, 5)));
    REQUIRE(result.size == rect.size);
    REQUIRE(result.placement.orientation.angle == 0);
}

TEST_CASE("OrientedRectangle: 3D rectangle with rotation")
{
    using V = Xyz::Vector3D;
    using R3D = Xyz::OrientedRectangle3D;
    const auto sq2 = sqrt(2) / 2;
    const auto sq3 = sqrt(3) / 2;
    const R3D rect({{10, 10, 10}, {PI / 4, PI / 6, PI / 2}}, {10, 10});
    REQUIRE(are_equal(rect.length_vector(), V(sq2 * sq3, sq2 * sq3, -0.5) * 10));
    REQUIRE(are_equal(rect.width_vector(), V(sqrt(25.0 / 2), sqrt(25.0 / 2), sq3 * 10)));
    REQUIRE(are_equal(rect.normal_vector(), V(sq2, -sq2, 0)));
}

TEST_CASE("OrientedRectangle: 3D rectangle without rotation")
{
    using V = Xyz::Vector3D;
    using R3D = Xyz::OrientedRectangle3D;
    const R3D rect({{1, -1, 1}, {0, 0, 0}}, {-2, 2});
    const auto o = rect.placement.origin;
    const auto [x, y] = Xyz::get_vectors(rect);
    REQUIRE(are_equal(o, V(1, -1, 1)));
    REQUIRE(are_equal(o + x, V(-1, -1, 1)));
    REQUIRE(are_equal(o + x + y, V(-1, 1, 1)));
    REQUIRE(are_equal(o + y, V(1, 1, 1)));
    REQUIRE(are_equal(rect.normal_vector(), V(0, 0, 1)));
}

TEST_CASE("OrientedRectangle: 3D rectangle with roll")
{
    using V = Xyz::Vector3D;
    using R3D = Xyz::OrientedRectangle3D;
    const R3D rect3({{1, 1, 1}, {0, 0, PI}}, {-2, 2});
    const auto o = rect3.placement.origin;
    const auto [x, y] = Xyz::get_vectors(rect3);
    REQUIRE(are_equal(o, V(1, 1, 1)));
    REQUIRE(are_equal(o + x, V(-1, 1, 1)));
    REQUIRE(are_equal(o + x + y, V(-1, -1, 1)));
    REQUIRE(are_equal(o + y, V(1, -1, 1)));
    REQUIRE(are_equal(rect3.normal_vector(), V(0, 0, -1)));
}
