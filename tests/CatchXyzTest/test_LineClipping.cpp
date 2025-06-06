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

void clip_succeeds(const Xyz::Vector2D& start,
                   const Xyz::Vector2D& end,
                   double expected_t0, double expected_t1)
{
    CAPTURE(start);
    CAPTURE(end);
    CAPTURE(expected_t0);
    CAPTURE(expected_t1);
    using Catch::Matchers::WithinAbs;
    const Xyz::LineSegment line_segment(start, end);
    auto ts = Xyz::get_clipping_positions(line_segment.start, line_segment.end);
    REQUIRE(ts.has_value());
    CHECK_THAT(ts->first, WithinAbs(expected_t0, 1e-12));
    CHECK_THAT(ts->second, WithinAbs(expected_t1, 1e-12));
}

void clip_fails(const Xyz::Vector2D& start,
                const Xyz::Vector2D& end)
{
    const Xyz::LineSegment line_segment(start, end);
    const auto ts = Xyz::get_clipping_positions(line_segment.start, line_segment.end);
    REQUIRE(!ts);
}

TEST_CASE("LineClipping", "[LineClipping]")
{
    SECTION("Line inside rectangle")
    {
        clip_succeeds({0.25, 0.25}, {0.75, 0.75}, 0, 1);
    }
    SECTION("Line outside rectangle")
    {
        clip_fails({-0.6, 0.5}, {0.5, 1.6});
    }
    SECTION("Partially inside rectangle")
    {
        clip_succeeds({-0.5, 0}, {1, 1.5}, 1 / 3.0, 2 / 3.0);
    }
    SECTION("Partially inside rectangle, reversed")
    {
        clip_succeeds({1, 1.5}, {-0.5, 0}, 1 / 3.0, 2 / 3.0);
    }
    SECTION("Begins inside rectangle")
    {
        clip_succeeds({0.5, 0.1}, {0.5, 1.3}, 0, 0.75);
    }
    SECTION("Ends inside rectangle")
    {
        clip_succeeds({0.5, 1.3}, {0.5, 0.1}, 0.25, 1.0);
    }
}
