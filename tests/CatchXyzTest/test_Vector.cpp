//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/SphericalPoint.hpp>
#include <catch2/catch.hpp>

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
