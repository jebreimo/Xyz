//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-12.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/AxisSystem.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("AxisSystem: operator-")
{
    using Xyz::Axis;
    CHECK(-Axis::X == Axis::NEG_X);
    CHECK(-Axis::Y == Axis::NEG_Y);
    CHECK(-Axis::Z == Axis::NEG_Z);
    CHECK(-Axis::NEG_X == Axis::X);
    CHECK(-Axis::NEG_Y == Axis::Y);
    CHECK(-Axis::NEG_Z == Axis::Z);
}

TEST_CASE("AxisSystem: is_negative")
{
    using Xyz::Axis;
    CHECK(!Xyz::is_negative(Axis::X));
    CHECK(!Xyz::is_negative(Axis::Y));
    CHECK(!Xyz::is_negative(Axis::Z));
    CHECK(Xyz::is_negative(Axis::NEG_X));
    CHECK(Xyz::is_negative(Axis::NEG_Y));
    CHECK(Xyz::is_negative(Axis::NEG_Z));
}

TEST_CASE("AxisSystem: AxisSwizzler")
{
    using Xyz::Axis;
    using Xyz::AxisSwizzler;
    using Xyz::Vector;

    constexpr Vector<float, 3> v{1, 2, 3};

    SECTION("Swizzle to {X, -Y, Z}")
    {
        constexpr AxisSwizzler<float, 3> swizzler{{0, 1, 2}, {1, -1, 1}};
        auto result = swizzler(v);
        CHECK_THAT(result[0], Catch::Matchers::WithinAbs(1, 1e-6));
        CHECK_THAT(result[1], Catch::Matchers::WithinAbs(-2, 1e-6));
        CHECK_THAT(result[2], Catch::Matchers::WithinAbs(3, 1e-6));
    }
    SECTION("Swizzle to {-Z, Y, -X}")
    {
        constexpr AxisSwizzler<float, 3> swizzler{{2, 1, 0}, {-1, 1, -1}};
        auto result = swizzler(v);
        CHECK_THAT(result[0], Catch::Matchers::WithinAbs(-3, 1e-6));
        CHECK_THAT(result[1], Catch::Matchers::WithinAbs(2, 1e-6));
        CHECK_THAT(result[2], Catch::Matchers::WithinAbs(-1, 1e-6));
    }
}

TEST_CASE("AxisSystem: AxisSystem constructor")
{
    using Xyz::Axis;
    using Xyz::AxisSystem;

    SECTION("Valid axis system {X, Y, Z}")
    {
        AxisSystem axis_system;
        auto [primary, secondary, up] = axis_system.get_axes();
        CHECK(primary == Axis::X);
        CHECK(secondary == Axis::Y);
        CHECK(up == Axis::Z);
    }

    SECTION("Valid axis system {Y, X, Z}")
    {
        AxisSystem axis_system(Axis::Y, Axis::X, Axis::Z);
        auto [primary, secondary, up] = axis_system.get_axes();
        CHECK(primary == Axis::Y);
        CHECK(secondary == Axis::X);
        CHECK(up == Axis::Z);
    }

    SECTION("Valid axis system {-Z, X, Y}")
    {
        AxisSystem axis_system(-Axis::Z, Axis::X, Axis::Y);
        auto [primary, secondary, up] = axis_system.get_axes();
        CHECK(primary == Axis::NEG_Z);
        CHECK(secondary == Axis::X);
        CHECK(up == Axis::Y);
    }

    SECTION("Invalid axis system: primary and secondary axes are the same")
    {
        CHECK_THROWS_AS(AxisSystem(Axis::X, Axis::X, Axis::Z), Xyz::XyzException);
    }

    SECTION("Invalid axis system: primary and up axes are the same")
    {
        CHECK_THROWS_AS(AxisSystem(Axis::X, Axis::Y, Axis::X), Xyz::XyzException);
    }

    SECTION("Invalid axis system: secondary and up axes are the same")
    {
        CHECK_THROWS_AS(AxisSystem(Axis::X, Axis::Y, Axis::Y), Xyz::XyzException);
    }
}
