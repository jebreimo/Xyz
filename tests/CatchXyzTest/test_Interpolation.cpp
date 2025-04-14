//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-04-14.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Interpolation.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("bilinear interpolation")
{
    using namespace Xyz;
    using Catch::Matchers::WithinAbs;

    auto p1 = Vector2F(10, 10);
    auto p2 = Vector2F(15, 15);
    Vector4F q(1, 2, 3, 4);

    REQUIRE_THAT(bilinear(q, p1, p2, {10, 10}), WithinAbs(1.0f, 0.00001f));
    REQUIRE_THAT(bilinear(q, p1, p2, {12.5, 10}), WithinAbs(1.5f, 0.00001f));
    REQUIRE_THAT(bilinear(q, p1, p2, {10, 12.5}), WithinAbs(2.0f, 0.00001f));
    REQUIRE_THAT(bilinear(q, p1, p2, {12.5f, 12.5f}), WithinAbs(2.5f, 0.00001f));
    REQUIRE_THAT(bilinear(q, p1, p2, {15, 12.5}), WithinAbs(3.0f, 0.00001f));
    REQUIRE_THAT(bilinear(q, p1, p2, {12.5, 15}), WithinAbs(3.5f, 0.00001f));
}
