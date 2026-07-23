//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/AABB.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
    using AABB2I = Xyz::AABB<int, 2>;
    using AABB3I = Xyz::AABB<int, 3>;
    using V2 = Xyz::Vector2I;
    using V3 = Xyz::Vector3I;
}

TEST_CASE("AABB: operator bool")
{
    // Default constructed box is invalid (min > max in every dimension).
    REQUIRE(!static_cast<bool>(AABB2I()));

    // A box constructed from a single point is valid (min == max).
    REQUIRE(static_cast<bool>(AABB2I(V2(1, 2))));

    // A box with min <= max in every dimension is valid.
    REQUIRE(static_cast<bool>(AABB2I(V2(0, 0), V2(10, 20))));

    // A box valid on one axis but inverted on another is invalid.
    REQUIRE(!static_cast<bool>(AABB2I(V2(0, 0), V2(10, -5))));
    REQUIRE(!static_cast<bool>(AABB2I(V2(0, 0), V2(-5, 10))));

    // Fully inverted box is invalid.
    REQUIRE(!static_cast<bool>(AABB2I(V2(10, 20), V2(0, 0))));
}

TEST_CASE("AABB: operator+= with AABB")
{
    AABB2I a(V2(0, 0), V2(10, 10));
    AABB2I b(V2(5, -5), V2(20, 8));
    a += b;
    REQUIRE(a.min == V2(0, -5));
    REQUIRE(a.max == V2(20, 10));

    // Returns a reference to the modified left operand.
    AABB2I c(V2(0, 0), V2(1, 1));
    AABB2I& ref = (c += AABB2I(V2(-1, -1), V2(2, 2)));
    REQUIRE(&ref == &c);
    REQUIRE(c.min == V2(-1, -1));
    REQUIRE(c.max == V2(2, 2));
}

TEST_CASE("AABB: operator+= with Vector")
{
    AABB2I a(V2(0, 0), V2(10, 10));
    a += V2(15, 5);
    REQUIRE(a.min == V2(0, 0));
    REQUIRE(a.max == V2(15, 10));

    a += V2(-3, 20);
    REQUIRE(a.min == V2(-3, 0));
    REQUIRE(a.max == V2(15, 20));

    // Adding a point already inside the box leaves it unchanged.
    a += V2(5, 5);
    REQUIRE(a.min == V2(-3, 0));
    REQUIRE(a.max == V2(15, 20));

    // Growing a default (empty) box by a point yields a degenerate box at that point.
    AABB2I empty;
    empty += V2(7, 8);
    REQUIRE(empty.min == V2(7, 8));
    REQUIRE(empty.max == V2(7, 8));

    // Returns a reference to the modified left operand.
    AABB2I& ref = (a += V2(0, 0));
    REQUIRE(&ref == &a);
}

TEST_CASE("AABB: operator+ AABB and AABB")
{
    AABB2I a(V2(0, 0), V2(10, 10));
    AABB2I b(V2(5, -5), V2(20, 8));
    AABB2I c = a + b;
    REQUIRE(c.min == V2(0, -5));
    REQUIRE(c.max == V2(20, 10));

    // Operands are left unchanged.
    REQUIRE(a.min == V2(0, 0));
    REQUIRE(a.max == V2(10, 10));
    REQUIRE(b.min == V2(5, -5));
    REQUIRE(b.max == V2(20, 8));
}

TEST_CASE("AABB: operator+ AABB and Vector")
{
    AABB2I a(V2(0, 0), V2(10, 10));
    AABB2I c = a + V2(15, -5);
    REQUIRE(c.min == V2(0, -5));
    REQUIRE(c.max == V2(15, 10));

    // Operand is left unchanged.
    REQUIRE(a.min == V2(0, 0));
    REQUIRE(a.max == V2(10, 10));
}

TEST_CASE("AABB: operator+ Vector and AABB")
{
    AABB2I a(V2(0, 0), V2(10, 10));
    AABB2I c = V2(15, -5) + a;
    REQUIRE(c.min == V2(0, -5));
    REQUIRE(c.max == V2(15, 10));

    // Commutative with the AABB + Vector overload.
    REQUIRE((V2(15, -5) + a).min == (a + V2(15, -5)).min);
    REQUIRE((V2(15, -5) + a).max == (a + V2(15, -5)).max);
}

TEST_CASE("AABB: operators work in 3 dimensions")
{
    AABB3I a(V3(0, 0, 0), V3(10, 10, 10));
    a += V3(-5, 12, 3);
    REQUIRE(a.min == V3(-5, 0, 0));
    REQUIRE(a.max == V3(10, 12, 10));

    AABB3I b = a + AABB3I(V3(1, 1, -8), V3(2, 2, 2));
    REQUIRE(b.min == V3(-5, 0, -8));
    REQUIRE(b.max == V3(10, 12, 10));

    REQUIRE(static_cast<bool>(b));
    REQUIRE(!static_cast<bool>(AABB3I()));
}