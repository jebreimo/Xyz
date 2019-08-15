//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-11.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Ytest/Ytest.hpp>
#include <Xyz/Vector.hpp>
#include <Xyz/Geometry/CoordinateSystem.hpp>

namespace
{
    void test_Roundtrip()
    {
        Xyz::CoordinateSystem<double> sys({1, 1, 0}, {1, 1, 0},
                                          {-1, 1, 0}, {0, 0, 2});
        Xyz::Vector3d p0 = {2, 0, 0.5};
        auto p1 = sys.fromWorldPos(p0);
        auto p2 = sys.toWorldPos(p1);
        Y_EQUAL(p0, p2);
    }

    Y_TEST(test_Roundtrip);
}
