//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-25.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/SphericalPoint.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

constexpr auto PI = Xyz::Constants<double>::PI;
constexpr auto MARGIN = Xyz::Constants<double>::DEFAULT_MARGIN;

using Catch::Matchers::WithinAbs;

void test_from_xyz_to_spherical_and_back(Xyz::Vector3D cartesian,
                                         Xyz::SphericalPoint<double> spherical)
{
    auto sp = Xyz::to_spherical<double>(cartesian);
    REQUIRE_THAT(sp.radius, WithinAbs(spherical.radius, MARGIN));
    REQUIRE_THAT(sp.azimuth, WithinAbs(spherical.azimuth, MARGIN));
    REQUIRE_THAT(sp.polar, WithinAbs(spherical.polar, MARGIN));
    auto p = Xyz::to_cartesian(sp);
    REQUIRE_THAT(p[0], WithinAbs(cartesian[0], MARGIN));
    REQUIRE_THAT(p[1], WithinAbs(cartesian[1], MARGIN));
    REQUIRE_THAT(p[2], WithinAbs(cartesian[2], MARGIN));
}

TEST_CASE("xyz to spherical")
{
    SECTION("0°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, 0, -1},
            {sqrt(2), 0, -PI / 4});
    }
    SECTION("45°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, 1, 1},
            {sqrt(3), PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("90°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {0, 1, -1},
            {sqrt(2), PI / 2, -PI / 4});
    }
    SECTION("135°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 1, 1},
            {sqrt(3), 3 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("180°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 0, -1},
            {sqrt(2), PI, -PI / 4});
    }
    SECTION("-135°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, -1, 1},
            {sqrt(3), -3 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("180°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 0, -1},
            {sqrt(2), PI, -PI / 4});
    }
    SECTION("-45°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, -1, 1},
            {sqrt(3), -1 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("-90°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {0, -1, -1},
            {sqrt(2), -PI / 2, -PI / 4});
    }
}

TEST_CASE("clamp vector")
{
    Xyz::Vector4D v1(1, -2, 3, -4);
    auto v2 = get_clamped(v1, -1, 1);
    REQUIRE(v2 == Xyz::Vector4D(1, -1, 1, -1));
    REQUIRE(v1 == Xyz::Vector4D(1, -2, 3, -4));
    clamp_inplace(v1, -1, 1);
    REQUIRE(v1 == Xyz::Vector4D(1, -1, 1, -1));
}

TEST_CASE("clamp vector with min and max vectors")
{
    using Vec = Xyz::Vector2D;
    constexpr Vec min(-2, 3);
    constexpr Vec max(8, 13);
    REQUIRE(get_clamped(Vec(-1, 4), min, max) == Vec(-1, 4));
    REQUIRE(get_clamped(Vec(-3, 4), min, max) == Vec(-2, 4));
    REQUIRE(get_clamped(Vec(9, 4), min, max) == Vec(8, 4));
    REQUIRE(get_clamped(Vec(8, 0), min, max) == Vec(8, 3));
    REQUIRE(get_clamped(Vec(-2, 14), min, max) == Vec(-2, 13));
}

TEST_CASE("scale vector")
{
    Xyz::Vector4D v1(1, -2, 3, -4);
    auto v2 = get_scaled(v1, 1);
    const auto root = std::sqrt(1 + 2*2 + 3*3 + 4*4);
    REQUIRE(are_equal(v2, Xyz::Vector4D(1 / root, -2 / root, 3 / root, -4 / root)));

    scale_inplace(v1, 1);
    REQUIRE(are_equal(v1, Xyz::Vector4D(1 / root, -2 / root, 3 / root, -4 / root)));
}

TEST_CASE("vector floor")
{
    Xyz::Vector4D v(1.2, -2.2, 3.2, -4.9);
    REQUIRE(floor(v) == Xyz::Vector4D(1, -3, 3, -5));
}

TEST_CASE("vector ceil")
{
    Xyz::Vector4D v(1.2, -2.2, 3.2, -4.9);
    REQUIRE(ceil(v) == Xyz::Vector4D(2, -2, 4, -4));
}
