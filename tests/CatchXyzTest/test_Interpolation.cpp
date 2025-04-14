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
    auto q11 = 1.f;
    auto q21 = 2.f;
    auto q12 = 3.f;
    auto q22 = 4.f;

    auto p = Vector2F(10, 10);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(1.0f, 0.00001f));

    p = Vector2F(12.5, 10);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(1.5f, 0.00001f));

    p = Vector2F(10, 12.5);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(2.0f, 0.00001f));

    p = Vector2F(12.5f, 12.5f);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(2.5f, 0.00001f));

    p = Vector2F(15, 12.5);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(3.0f, 0.00001f));

    p = Vector2F(12.5, 15);
    REQUIRE_THAT(bilinear(p, p1, p2, q11, q21, q12, q22), WithinAbs(3.5f, 0.00001f));
}
