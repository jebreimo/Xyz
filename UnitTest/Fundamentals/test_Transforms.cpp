//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Xyz.hpp"

#include "../../Externals/Ytest/Ytest.hpp"
#include "../../Xyz/Utilities/Utilities.hpp"

namespace H = Xyz::HomogeneousTranforms;
namespace TH =  Xyz::TransposedHomogeneousTransforms;

namespace Ytest
{
    template <typename T, typename U, unsigned N>
    bool equal(const Xyz::Vector<T, N>& v, const Xyz::Vector<U, N>& u)
    {
        return Xyz::areEquivalent(v, u, 1e-9);
    }
}

void test_HomoRotateZ4()
{
    auto trans = H::rotateZ4(Xyz::toRadians(90.0));
    auto v = trans * Xyz::vector4(1, 1, 1, 1);
    Y_EQUAL(v, Xyz::vector4(-1, 1, 1, 1));
}

void test_HomoTransposeZ4()
{
    auto trans = H::translate4(Xyz::vector3(1.0, 2.0, 3.0));
    auto v = trans * Xyz::vector4(1, 1, 1, 1);
    Y_EQUAL(v, Xyz::vector4(2, 3, 4, 1));
}

void test_Something()
{
    auto trans = multiplyTransposed(H::rotateZ4(Xyz::toRadians(90.0)),
                                    TH::translate4(Xyz::vector3(1, 2, 3)));
    auto v = trans * Xyz::vector4(1, 1, 1, 1);
    Y_EQUAL(v, Xyz::vector4(-3, 2, 4, 1));
}

Y_SUBTEST("Fundamentals",
          test_HomoRotateZ4,
          test_HomoTransposeZ4,
          test_Something);
