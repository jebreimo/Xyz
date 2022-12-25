//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Vector.hpp>
#include <catch2/catch.hpp>

void test_from_xyz_to_spherical_and_back(Xyz::Vector3D xyz,
                                         Xyz::Vector3D spherical)
{
    auto sp = Xyz::xyz_to_spherical<double>(xyz);
    REQUIRE(sp[0] == Approx(spherical[0]));
    REQUIRE(sp[1] == Approx(spherical[1]));
    REQUIRE(sp[2] == Approx(spherical[2]));
    auto p = Xyz::spherical_to_xyz(sp);
    REQUIRE(p[0] == Approx(xyz[0]));
    REQUIRE(p[1] == Approx(xyz[1]));
    REQUIRE(p[2] == Approx(xyz[2]));
}

TEST_CASE("xyz to spherical")
{
    SECTION("45°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, 1, 1},
            {sqrt(3), Xyz::Constants<double>::PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("135°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 1, 1},
            {sqrt(3), 3 * Xyz::Constants<double>::PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("225°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, -1, 1},
            {sqrt(3), -3 * Xyz::Constants<double>::PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("-45°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, -1, 1},
            {sqrt(3), -1 * Xyz::Constants<double>::PI / 4, asin(1 / sqrt(3))});
    }
}
