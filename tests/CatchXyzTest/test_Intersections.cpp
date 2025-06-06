//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 13.02.2016
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test get_intersection_positions")
{
    using V2 = Xyz::Vector2I;
    auto a = Xyz::LineSegment(V2(-4, -2), V2(4, 2));
    auto b = Xyz::LineSegment(V2(3, -9), V2(-1, 3));
    auto intersection = get_intersection_positions(a, b, 1e-10);
    CHECK(std::get<0>(intersection) == Xyz::LineRelationship::INTERSECTING);
    CHECK(std::get<1>(intersection) == 0.5);
    CHECK(std::get<2>(intersection) == 0.75);
}
