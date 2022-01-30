//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-01-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Generators.hpp>
#include <catch2/catch.hpp>
#include <algorithm>

TEST_CASE("Test rectangle generator")
{
    constexpr auto PI = Xyz::Constants<double>::PI;
    auto iterable = Xyz::CircleGenerator<double>(4).start_angle(PI / 4);
    std::vector<Xyz::Vector<double, 2>> points;
    std::copy(iterable.begin(), iterable.end(), std::back_inserter(points));
    const auto SIDE = std::sqrt(1.0 / 2.0);
    REQUIRE(points.size() == 4);
    REQUIRE(Xyz::are_equivalent(points[0], Xyz::make_vector2(SIDE, SIDE)));
    REQUIRE(Xyz::are_equivalent(points[1], Xyz::make_vector2(-SIDE, SIDE)));
    REQUIRE(Xyz::are_equivalent(points[2], Xyz::make_vector2(-SIDE, -SIDE)));
    REQUIRE(Xyz::are_equivalent(points[3], Xyz::make_vector2(SIDE, -SIDE)));
}
