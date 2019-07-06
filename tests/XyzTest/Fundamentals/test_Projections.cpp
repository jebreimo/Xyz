//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace {

    void test_PerspectiveProjection()
    {
        auto m = Xyz::frustum<double>(-1, 1, -1, 1, 9, 11);
        auto v = Xyz::makeVector<double>(-1.0, -1.0, -9.0, 1.0);
        auto w = m * v;
        Y_EQUAL(w, Xyz::makeVector<double>(-9, -9, -9, 9));
    }

    void test_LookAt()
    {
        auto m = Xyz::lookAt<double>(Xyz::makeVector(5, 2, 3),
                                     Xyz::makeVector(1, 8, 3),
                                     Xyz::makeVector(0, 0, 1));
        auto result = m * Xyz::makeVector(1.5, 4.0, 3.0, 1.0);
        auto expected = Xyz::makeVector(-std::sqrt(1 + 1.5 * 1.5),
                                        0.0,
                                        -std::sqrt(2 * 2 + 3 * 3),
                                        1.0);
        Y_EQUAL(result, expected);
    }

    Y_SUBTEST("Fundamentals",
              test_PerspectiveProjection,
              test_LookAt);
}
