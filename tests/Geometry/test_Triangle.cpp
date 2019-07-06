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
        auto triangle = Xyz::makeTriangle(Xyz::makeVector(-5, 0),
                                          Xyz::makeVector(5, 0),
                                          Xyz::makeVector(0, 5));
        Y_ASSERT(containsPoint(triangle, Xyz::makeVector(0, 2), 1e-12));
        Y_ASSERT(!containsPoint(triangle, Xyz::makeVector(-3, 2), 1e-12));
    }

    void test_containsPointInclusive()
    {
        auto tri = Xyz::makeTriangle(Xyz::makeVector(-5, 0),
                                     Xyz::makeVector(5, 0),
                                     Xyz::makeVector(0, 5));
        Y_ASSERT(containsPointInclusive(tri, Xyz::makeVector(0, 2), 1e-12));
        Y_ASSERT(containsPointInclusive(tri, Xyz::makeVector(-3, 2), 1e-12));
    }

    Y_SUBTEST("Geometry",
              test_containsPoint,
              test_containsPointInclusive);
}
