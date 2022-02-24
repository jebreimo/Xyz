//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/MatrixDeterminant.hpp"

#include "Ytest/Ytest.hpp"

namespace
{
    using namespace Xyz;

    void test_get_determinant3()
    {
        Matrix3I m {1, 4, 3,
                    2, 1, 5,
                    3, 2, 1};
        Y_EQUAL(get_determinant(m), 46);
    }

    void test_get_determinant4()
    {
        Matrix4I m{1, -2, 3, 2,
                   2, 3, 1, -1,
                   3, 3, 3, 3,
                   -1, 4, 2, 1};
        Y_EQUAL(get_determinant(m), -141);
    }

    Y_TEST(test_get_determinant3,
           test_get_determinant4);
}
