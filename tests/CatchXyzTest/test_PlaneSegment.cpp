//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-19.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/PlaneSegment.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Invalid plane segment")
{
    Xyz::PlaneSegment<float> plane;
    REQUIRE(!plane.is_valid());
    plane = Xyz::PlaneSegment<float>({0, 0, 0}, {1, 0, 0}, {-2, 0, 0});
    REQUIRE(!plane.is_valid());
}

TEST_CASE("Size of plane segment")
{
    Xyz::PlaneSegment<float> plane({0, 0, 0}, {-3, 4, 0}, {4, 3, 0});
    REQUIRE(plane.size() == Xyz::Vector2F(5, 5));
}
