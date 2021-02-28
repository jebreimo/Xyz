//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/LinearAlgebra.hpp"

#include "Ytest/Ytest.hpp"

namespace
{
    using namespace Xyz;

    void test_GetDeterminant3()
    {
        Matrix3i m {1, 4, 3,
                    2, 1, 5,
                    3, 2, 1};
        Y_EQUAL(getDeterminant(m), 46);
    }

    void test_GetDeterminant4()
    {
        Matrix4i m{1, -2, 3, 2,
                   2, 3, 1, -1,
                   3, 3, 3, 3,
                   -1, 4, 2, 1};
        Y_EQUAL(getDeterminant(m), -141);
    }

    Y_TEST(test_GetDeterminant3,
           test_GetDeterminant4);
}
