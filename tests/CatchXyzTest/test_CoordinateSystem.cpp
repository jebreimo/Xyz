//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-11.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/CoordinateSystem.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
    TEST_CASE("CoordinateSystem rotated XY", "[CoordinateSystem]")
    {
        Xyz::CoordinateSystem<double> sys({1, 1, 0}, {1, 1, 0},
                                          {-1, 1, 0}, {0, 0, 2});
        Xyz::Vector3D p0 = {2, 0, 0.5};
        auto p1 = sys.from_world_pos(p0);
        auto p2 = sys.to_world_pos(p1);
        REQUIRE(p0 == p2);
    }

    TEST_CASE("CoordinateSystem YZX", "[CoordinateSystem]")
    {
        Xyz::CoordinateSystem<double> sys({0, 0, 0}, {0, 1, 0},
                                          {0, 0, 1}, {1, 0, 0});
        Xyz::Vector3D p1 = {-1, -1, 1};
        auto p2 = sys.to_world_pos(p1);
        REQUIRE(p2 == Xyz::make_vector3(1, -1, -1));
    }
}
