//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace {
    void test_RotateZ()
    {
        auto trans = Xyz::rotateZ(Xyz::toRadians(90.0));
        auto v = trans * Xyz::makeVector<double>(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector<double>(-1, 1, 1, 1));
    }

    void test_Translate4()
    {
        auto trans = Xyz::translate4(1.0, 2.0, 3.0);
        auto v = trans * Xyz::makeVector<double>(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector<double>(2, 3, 4, 1));
    }

    void test_RotateZ_and_TransposedTranslate4()
    {
        auto trans = multiplyTransposed(
                Xyz::rotateZ(Xyz::toRadians(90.0)),
                Xyz::transposedTranslate4<double>(1, 2, 3));
        auto v = trans * Xyz::makeVector<double>(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector<double>(-3, 2, 4, 1));
    }

    Y_SUBTEST("Fundamentals",
              test_RotateZ,
              test_Translate4,
              test_RotateZ_and_TransposedTranslate4);

}
