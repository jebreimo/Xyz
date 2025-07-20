//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-06-05.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Xyz/LineClipper.hpp"

TEST_CASE("Test 2D line-clipper with scale and translate transform",
          "[LineClipper][2D]")
{
    auto transform = Xyz::affine::scale2<float>(0.05f, 0.05f) *
        Xyz::affine::translate2<float>(10.f, 10.f);
    const Xyz::LineClipper<float, 2> clipper(transform);
    using V = Xyz::Vector<float, 2>;
    SECTION("Partially inside rectangle")
    {
        const auto s = clipper.clip(Xyz::LineSegment(V(-20, -10), V(10, 20)));
        REQUIRE(s.has_value());
        CHECK(Xyz::are_equal(s->start, V(-10, 0)));
        CHECK(Xyz::are_equal(s->end, V(0, 10)));
    }
}
