//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 24.04.2017.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

void clip_succeeds(const Xyz::Rectangle<double>& rectangle,
                   const Xyz::Vector2D& start,
                   const Xyz::Vector2D& end,
                   double expected_t0, double expected_t1)
{
    using Catch::Matchers::WithinAbs;
    Xyz::LineSegment<double, 2> line_segment(start, end);
    auto ts = Xyz::get_clipping_positions(rectangle, line_segment);
    REQUIRE(ts.has_value());
    CHECK_THAT(ts->first, WithinAbs(expected_t0, 1e-12));
    CHECK_THAT(ts->second, WithinAbs(expected_t1, 1e-12));
}

void clip_fails(const Xyz::Rectangle<double>& rectangle,
                const Xyz::Vector2D& start,
                const Xyz::Vector2D& end)
{
    Xyz::LineSegment<double, 2> line_segment(start, end);
    auto ts = Xyz::get_clipping_positions(rectangle, line_segment);
    REQUIRE(!ts);
}

TEST_CASE("LineClipping", "[LineClipping]")
{
    Xyz::Rectangle<double> rect({-10, -10}, {20, 20});
    SECTION("Line inside rectangle")
    {
        clip_succeeds(rect, {-5, -5}, {5, 5}, 0, 1);
    }
    SECTION("Line outside rectangle")
    {
        clip_fails(rect, {-22, 0}, {0, 22});
    }
    SECTION("Partially inside rectangle")
    {
        clip_succeeds(rect, {-20, -10}, {10, 20}, 1 / 3.0, 2 / 3.0);
    }
    SECTION("Partially inside rectangle, reversed")
    {
        clip_succeeds(rect, {10, 20}, {-20, -10}, 1 / 3.0, 2 / 3.0);
    }
    SECTION("Begins inside rectangle")
    {
        clip_succeeds(rect, {0, -8}, {0, 16}, 0, 0.75);
    }
    SECTION("Ends inside rectangle")
    {
        clip_succeeds(rect, {0, 16}, {0, -8}, 0.25, 1.0);
    }
}
