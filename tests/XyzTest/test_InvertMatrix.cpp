//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-05.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/InvertMatrix.hpp"
#include "Ytest/Ytest.hpp"

namespace
{
    using namespace Xyz;

    template <typename T, unsigned N>
    bool equals_identity_matrix(const Matrix<T, N, N>& m)
    {
        return are_equal(m, make_identity_matrix<T, N>());
    }

    void test_invert2()
    {
        Matrix2D m{1, 4,
                   2, 1};
        Y_ASSERT(equals_identity_matrix(m * invert(m)));
    }

    void test_invert3()
    {
        Matrix3D m{1, 4, 2,
                   2, 1, 3,
                   5, 7, 2};
        Y_ASSERT(equals_identity_matrix(m * invert(m)));
    }

    void test_invert4()
    {
        Matrix4D m{1, -2, 3, 2,
                   2, 3, 1, -1,
                   3, 7, 0, 3,
                   -1, 4, 2, 1};
        Y_ASSERT(equals_identity_matrix(m * invert(m)));
    }

    Y_TEST(test_invert2,
           test_invert3,
           test_invert4);
}
