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
        const Xyz::CoordinateSystem<double> cs({1, 1, 0}, {1, 1, 0},
                                          {-1, 1, 0}, {0, 0, 2});
        REQUIRE(cs.origin() == Xyz::Vector3D(1, 1, 0));
        REQUIRE(cs.x_axis() == Xyz::Vector3D(1, 1, 0));
        REQUIRE(cs.y_axis() == Xyz::Vector3D(-1, 1, 0));
        REQUIRE(cs.z_axis() == Xyz::Vector3D(0, 0, 2));
        Xyz::Vector3D p0 = {2, 0, 0.5};
        auto p1 = cs.from_cs(p0);
        auto p2 = cs.to_cs(p1);
        REQUIRE(p0 == p2);
    }

    TEST_CASE("CoordinateSystem z is off", "[CoordinateSystem]")
    {
        using V2 = Xyz::Vector2D;
        using V3 = Xyz::Vector3D;
        Xyz::CoordinateSystem<double> cs({0, 0, 0}, {1, 0, 0},
                                          {0, 1, 0}, {-1, -1, 4});
        REQUIRE(is_valid(cs));
        REQUIRE(are_equal(cs.to_cs_xy({0, 0, 4}), V2(1, 1)));
        REQUIRE(are_equal(cs.from_cs_xy({2, 2}), V3(2, 2, 0)));
    }

    TEST_CASE("CoordinateSystem YZX", "[CoordinateSystem]")
    {
        Xyz::CoordinateSystem<double> sys({0, 0, 0}, {0, 1, 0},
                                          {0, 0, 1}, {1, 0, 0});
        Xyz::Vector3D p1 = {-1, -2, 3};
        auto p2 = sys.from_cs(p1);
        REQUIRE(p2 == Xyz::make_vector3(3, -1, -2));
        auto p3 = sys.to_cs(p1);
        REQUIRE(p3 == Xyz::make_vector3(-2, 3, -1));
    }

    TEST_CASE("CoordinateSystem from invalid plane", "[CoordinateSystem]")
    {
        const auto cs = Xyz::make_coordinate_system<double>({{1, 1, 0}, {0, 0, 0}});
        REQUIRE(!cs);
    }

    TEST_CASE("CoordinateSystem from XY plane 2D", "[CoordinateSystem]")
    {
        using V = Xyz::Vector3D;
        const auto cs = Xyz::make_coordinate_system<double>({{1, 1, 0}, {-1, -1, 1}});
        REQUIRE(cs);
        REQUIRE(is_valid(*cs));
        REQUIRE(cs->origin() == V(2, 0, 0));
        REQUIRE(are_equal(cs->x_axis(), V(1 / sqrt(2), -1 / sqrt(2), 0)));
        REQUIRE(are_equal(cs->y_axis(), V(1 / sqrt(6), 1 / sqrt(6), sqrt(2. / 3))));
        REQUIRE(are_equal(cs->z_axis(), V(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3))));
    }
}
