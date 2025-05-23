//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Matrix.hpp"

#include "Ytest/Ytest.hpp"
#include "Xyz/Vector.hpp"
//#include "Xyz/Utilities/Utilities.hpp"

namespace
{
    using namespace Xyz;

    void test_basics()
    {
        auto mat = Matrix3D{1, 0, 0,
                            0, 1, 0,
                            0, 0, 1};
        Y_EQUAL(mat[RowCol(0, 0)], 1.0);
        Y_EQUAL(mat[RowCol(0, 1)], 0.0);
        Y_EQUAL(mat[RowCol(1, 1)], 1.0);
    }

    void test_transpose()
    {
        auto mat1 = Matrix4D{0, 1, 2, 3,
                             4, 5, 6, 7,
                             8, 9, 0, 1,
                             2, 3, 4, 5};
        auto mat1t = Matrix4D{0, 4, 8, 2,
                              1, 5, 9, 3,
                              2, 6, 0, 4,
                              3, 7, 1, 5};
        Y_EQUAL(transpose(mat1), mat1t);
        transpose_inplace(mat1t);
        Y_EQUAL(mat1, mat1t);
    }

    void test_matrix_matrix_addition()
    {
        auto m1 = Matrix3I{1, 2, 3,
                           4, 5, 6,
                           7, 8, 9};
        auto m2 = Matrix3I{ 1, -1,  1,
                            -1,  1, -1,
                            1, -1,  1};
        auto product = Matrix3I{ 2,  1,  4,
                                 3,  6,  5,
                                 8,  7, 10};
        Y_EQUAL(m1 + m2, product);
    }

    void test_matrix_matrix_subtraction()
    {
        auto m1 = Matrix3I{1, 2, 3,
                           4, 5, 6,
                           7, 8, 9};
        auto m2 = Matrix3I{ 1, -1,  1,
                            -1,  1, -1,
                            1, -1,  1};
        auto product = Matrix3I{0, 3, 2,
                                5, 4, 7,
                                6, 9, 8};
        Y_EQUAL(m1 - m2, product);
    }

    void test_matrix_matrix_multiplication()
    {
        auto m1 = Matrix2I{1, 2,
                           3, 4};
        auto m2 = Matrix2I{4, 3,
                           1, 2};
        auto product = Matrix2I{ 6,  7,
                                 16, 17};
        Y_EQUAL(m1 * m2, product);
    }

    void test_matrix_vector_multiplication()
    {
        auto m = Matrix3I{1, 2, 3,
                          4, 5, 6,
                          7, 8, 9};
        auto v = Vector3I{1, 2, 3};
        auto product1 = Vector3I{14, 32, 50};
        auto product2 = Vector3I{30, 36, 42};
        Y_EQUAL(m * v, product1);
        Y_EQUAL(v * m, product2);
    }

    void test_make_submatrix()
    {
        Matrix4I m{1, 2, 3, 4,
                   5, 6, 7, 8,
                   9, 0, 1, 2,
                   3, 4, 5, 6};
        auto s = make_submatrix<3, 3>(m, 3, 2);
        Matrix3I e{5, 6, 3,
                   3, 4, 1,
                   7, 8, 5};
        Y_EQUAL(s, e);
    }

    void test_identity()
    {
        auto m1 = make_identity_matrix<int, 4>();
        Y_EQUAL(m1[RowCol(0, 0)], 1);
        Y_EQUAL(m1[RowCol(0, 1)], 0);
    }

    Y_SUBTEST("Fundamentals",
              test_basics,
              test_transpose,
              test_matrix_matrix_addition,
              test_matrix_matrix_subtraction,
              test_matrix_matrix_multiplication,
              test_matrix_vector_multiplication,
              test_make_submatrix,
              test_identity);
}
