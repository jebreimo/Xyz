//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 13.02.2016
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>

#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("get_intersection_positions")
{
    using V2 = Xyz::Vector2I;
    const auto a = Xyz::LineSegment(V2(-4, -2), V2(4, 2));
    const auto b = Xyz::LineSegment(V2(3, -9), V2(-1, 3));
    auto intersection = get_intersection_positions(a, b, 1e-10);
    CHECK(std::get<0>(intersection) == Xyz::IntersectionType::INTERSECTING);
    CHECK(std::get<1>(intersection) == 0.5);
    CHECK(std::get<2>(intersection) == 0.75);
}

TEST_CASE("get_intersection_extents")
{
    using V2 = Xyz::Vector2F;
    const Xyz::LineSegment a(V2(-1, -1), V2(1, 1.f / 3.f));
    const Xyz::LineSegment b(V2(-1.f / 6.f, -4.f / 9), V2(2, 1));
    auto [type, extent_a, extent_b] = get_intersection_extents(a, b);
    REQUIRE(type == Xyz::IntersectionType::INTERSECTING);
    CHECK_THAT(extent_a.first, WithinAbs(5.f / 12.f, 1e-6f));
    CHECK_THAT(extent_a.second, WithinAbs(1.f, 1e-6f));
    CHECK_THAT(extent_b.first, WithinAbs(0.f, 1e-6f));
    CHECK_THAT(extent_b.second, WithinAbs(7.f / 13.f, 1e-6f));
}
