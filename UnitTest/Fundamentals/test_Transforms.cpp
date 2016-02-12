//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace {
    namespace H = Xyz::HomogeneousTransforms;
    namespace TH =  Xyz::TransposedHomogeneousTransforms;

    void test_HomoRotateZ4()
    {
        auto trans = H::rotateZ4(Xyz::toRadians(90.0));
        auto v = trans * Xyz::makeVector(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector(-1, 1, 1, 1));
    }

    void test_HomoTransposeZ4()
    {
        auto trans = H::translate4(Xyz::makeVector(1.0, 2.0, 3.0));
        auto v = trans * Xyz::makeVector(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector(2, 3, 4, 1));
    }

    void test_Something()
    {
        auto trans = multiplyTransposed(H::rotateZ4(Xyz::toRadians(90.0)),
                                        TH::translate4(
                                                Xyz::makeVector(1, 2, 3)));
        auto v = trans * Xyz::makeVector(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::makeVector(-3, 2, 4, 1));
    }

    Y_SUBTEST("Fundamentals",
              test_HomoRotateZ4,
              test_HomoTransposeZ4,
              test_Something);

}
