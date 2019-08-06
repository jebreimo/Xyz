//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Matrix.hpp"

#include "Ytest/Ytest.hpp"

namespace
{
    using namespace Xyz;

    template <typename T, unsigned N>
    bool equalsIdentityMatrix(const Matrix<T, N, N>& m)
    {
        return areEquivalent(m, makeIdentityMatrix<T, N>());
    }

    void test_Invert2()
    {
        Matrix2d m{
                1, 4,
                2, 1};
        Y_ASSERT(equalsIdentityMatrix(m * invert(m)));
    }

    void test_Invert3()
    {
        Matrix3d m{
                1, 4, 2,
                2, 1, 3,
                5, 7, 2};
        Y_ASSERT(equalsIdentityMatrix(m * invert(m)));
    }

    //void test_GetDeterminant4()
    //{
    //    Matrix4i m{
    //            1, -2, 3, 2,
    //            2, 3, 1, -1,
    //            3, 3, 3, 3,
    //            -1, 4, 2, 1};
    //    Y_EQUAL(getDeterminant(m), -141);
    //}

    Y_TEST(test_Invert2,
           test_Invert3);
}
