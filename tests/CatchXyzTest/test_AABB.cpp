//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/AABB.hpp>
#include <Xyz/TransformationMatrix.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
    using AABB2I = Xyz::AABB<int, 2>;
    using AABB3I = Xyz::AABB<int, 3>;
    using V2 = Xyz::Vector2I;
    using V3 = Xyz::Vector3I;
    using V3F = Xyz::Vector3F;
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

TEST_CASE("AABB: transform_aabb translation (3D float)")
{
    Xyz::AABB3F box(V3F(0, 0, 0), V3F(2, 4, 6));
    auto result = transform_aabb(box, Xyz::affine::translate3(10.f, -5.f, 3.f));
    REQUIRE(are_equal(result.min, V3F(10, -5, 3)));
    REQUIRE(are_equal(result.max, V3F(12, -1, 9)));
}

TEST_CASE("AABB: transform_aabb scaling with axis flip (3D float)")
{
    // A negative scale factor flips an axis, so min/max on that axis swap.
    // transform_aabb must recompute the bounds from the transformed corners.
    Xyz::AABB3F box(V3F(1, 2, 3), V3F(3, 4, 5));
    auto result = transform_aabb(box, Xyz::affine::scale3(2.f, 0.5f, -1.f));
    REQUIRE(are_equal(result.min, V3F(2, 1, -5)));
    REQUIRE(are_equal(result.max, V3F(6, 2, -3)));
}

TEST_CASE("AABB: transform_aabb rotation grows the box (3D float)")
{
    // 90 degree rotation about the z axis: (x, y, z) -> (-y, x, z).
    const Xyz::Matrix4F rot_z_90 = {
        0, -1, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    Xyz::AABB3F box(V3F(0, 0, 0), V3F(2, 4, 6));
    auto result = transform_aabb(box, rot_z_90);
    REQUIRE(are_equal(result.min, V3F(-4, 0, 0)));
    REQUIRE(are_equal(result.max, V3F(0, 2, 6)));
}

TEST_CASE("AABB: transform_aabb composed transform (3D float)")
{
    // Scale by 2, then translate by (1, 2, 3). Matrices apply right-to-left.
    const auto m = Xyz::affine::translate3(1.f, 2.f, 3.f)
                   * Xyz::affine::scale3(2.f, 2.f, 2.f);
    Xyz::AABB3F box(V3F(0, 0, 0), V3F(1, 1, 1));
    auto result = transform_aabb(box, m);
    REQUIRE(are_equal(result.min, V3F(1, 2, 3)));
    REQUIRE(are_equal(result.max, V3F(3, 4, 5)));
}

TEST_CASE("AABB: transform_aabb of an invalid box stays invalid (3D float)")
{
    Xyz::AABB3F box;  // default-constructed: min > max, i.e. invalid/empty
    REQUIRE(!static_cast<bool>(box));
    auto result = transform_aabb(box, Xyz::affine::translate3(1.f, 2.f, 3.f));
    REQUIRE(!static_cast<bool>(result));
}

TEST_CASE("AABB: transform_aabb_no_w translation (3D float)")
{
    Xyz::AABB3F box(V3F(0, 0, 0), V3F(2, 4, 6));
    auto result = transform_aabb_no_w(box, Xyz::affine::translate3(10.f, -5.f, 3.f));
    REQUIRE(are_equal(result.min, V3F(10, -5, 3)));
    REQUIRE(are_equal(result.max, V3F(12, -1, 9)));
}

TEST_CASE("AABB: transform_aabb_no_w scaling with axis flip (3D float)")
{
    // A negative scale factor flips an axis, so min/max on that axis swap.
    Xyz::AABB3F box(V3F(1, 2, 3), V3F(3, 4, 5));
    auto result = transform_aabb_no_w(box, Xyz::affine::scale3(2.f, 0.5f, -1.f));
    REQUIRE(are_equal(result.min, V3F(2, 1, -5)));
    REQUIRE(are_equal(result.max, V3F(6, 2, -3)));
}

TEST_CASE("AABB: transform_aabb_no_w rotation grows the box (3D float)")
{
    // 90 degree rotation about the z axis: (x, y, z) -> (-y, x, z).
    const Xyz::Matrix4F rot_z_90 = {
        0, -1, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    Xyz::AABB3F box(V3F(0, 0, 0), V3F(2, 4, 6));
    auto result = transform_aabb_no_w(box, rot_z_90);
    REQUIRE(are_equal(result.min, V3F(-4, 0, 0)));
    REQUIRE(are_equal(result.max, V3F(0, 2, 6)));
}

TEST_CASE("AABB: transform_aabb_no_w matches transform_aabb for affine matrices (3D float)")
{
    // With an affine matrix (last row 0,0,0,1) w is always 1, so the optimized
    // no-w version must agree with the general transform_aabb.
    const auto m = Xyz::affine::translate3(1.f, 2.f, 3.f)
                   * Xyz::affine::scale3(2.f, 2.f, 2.f);
    Xyz::AABB3F box(V3F(-1, -2, -3), V3F(1, 2, 3));

    auto general = transform_aabb(box, m);
    auto no_w = transform_aabb_no_w(box, m);
    REQUIRE(are_equal(no_w.min, general.min));
    REQUIRE(are_equal(no_w.max, general.max));
}

TEST_CASE("AABB: transform_aabb_no_w of an invalid box stays invalid (3D float)")
{
    Xyz::AABB3F box;  // default-constructed: min > max, i.e. invalid/empty
    REQUIRE(!static_cast<bool>(box));
    auto result = transform_aabb_no_w(box, Xyz::affine::translate3(1.f, 2.f, 3.f));
    REQUIRE(!static_cast<bool>(result));
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