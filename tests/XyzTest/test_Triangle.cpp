//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 06.03.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include "Ytest/Ytest.hpp"

namespace
{
    void test_containsPoint()
    {
        auto triangle = Xyz::makeTriangle(Xyz::makeVector2(-5, 0),
                                          Xyz::makeVector2(5, 0),
                                          Xyz::makeVector2(0, 5));
        Y_ASSERT(containsPoint(triangle, Xyz::makeVector2(0, 2), 1e-12));
        Y_ASSERT(!containsPoint(triangle, Xyz::makeVector2(-3, 2), 1e-12));
    }

    void test_containsPointInclusive()
    {
        auto tri = Xyz::makeTriangle(Xyz::makeVector2(-5, 0),
                                     Xyz::makeVector2(5, 0),
                                     Xyz::makeVector2(0, 5));
        Y_ASSERT(containsPointInclusive(tri, Xyz::makeVector2(0, 2), 1e-12));
        Y_ASSERT(containsPointInclusive(tri, Xyz::makeVector2(-3, 2), 1e-12));
    }

    void test_getArea()
    {
        auto tri = Xyz::makeTriangle(Xyz::makeVector3(3, 1, 2),
                                     Xyz::makeVector3(7, 5, 2),
                                     Xyz::makeVector3(3, 7, 2));
        Y_EQUIVALENT(Xyz::getAreaSquared(tri), 144, 1e-10);
        Y_EQUIVALENT(Xyz::getArea(tri), 12, 1e-10);
    }

    Y_SUBTEST("Geometry",
              test_containsPoint,
              test_containsPointInclusive,
              test_getArea);
}
