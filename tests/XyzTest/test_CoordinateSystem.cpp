//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-11.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Ytest/Ytest.hpp>
#include <Xyz/Vector.hpp>
#include <Xyz/CoordinateSystem.hpp>

namespace
{
    void test_roundtrip()
    {
        Xyz::CoordinateSystem<double> sys({1, 1, 0}, {1, 1, 0},
                                          {-1, 1, 0}, {0, 0, 2});
        Xyz::Vector3d p0 = {2, 0, 0.5};
        auto p1 = sys.from_world_pos(p0);
        auto p2 = sys.to_world_pos(p1);
        Y_EQUAL(p0, p2);
    }

    void test_something()
    {
        Xyz::CoordinateSystem<double> sys({0, 0, 0}, {0, 1, 0},
                                          {0, 0, 1}, {1, 0, 0});
        Xyz::Vector3d p1 = {-1, -1, 1};
        auto p2 = sys.to_world_pos(p1);
        Y_EQUAL(p2, Xyz::make_vector3(1, -1, -1));
    }

    Y_TEST(test_roundtrip, test_something);
}
