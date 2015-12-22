//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Matrix.hpp"

#include <cmath>
#include "../../Externals/Ytest/Ytest.hpp"
//#include "../../Xyz/Utilities/Utilities.hpp"

namespace {

using namespace Xyz;

void test_Basics()
{
    auto mat = Matrix<double, 3>{1, 0, 0,
                                 0, 1, 0,
                                 0, 0, 1};
    Y_EQUAL(mat[0][0], 1);
    Y_EQUAL(mat[0][1], 0);
    Y_EQUAL(mat[1][1], 1);
}

void test_Transpose()
{
    auto mat1 = Matrix<double, 4>{0, 1, 2, 3,
                                  4, 5, 6, 7,
                                  8, 9, 0, 1,
                                  2, 3, 4, 5};
    auto mat1t = Matrix<double, 4>{0, 4, 8, 2,
                                   1, 5, 9, 3,
                                   2, 6, 0, 4,
                                   3, 7, 1, 5};
    Y_EQUAL(transposed(mat1), mat1t);
    transpose(mat1t);
    Y_EQUAL(mat1, mat1t);
}

void test_MatrixMatrixAddition()
{
    typedef Matrix<int, 3> Mat;
    auto m1 = Mat{1, 2, 3,
                  4, 5, 6,
                  7, 8, 9};
    auto m2 = Mat{ 1, -1,  1,
                  -1,  1, -1,
                   1, -1,  1};
    auto product =  Mat{ 2,  1,  4,
                         3,  6,  5,
                         8,  7, 10};
    Y_EQUAL(m1 + m2, product);
}

void test_MatrixMatrixSubtraction()
{
    typedef Matrix<int, 3> Mat;
    auto m1 = Mat{1, 2, 3,
                  4, 5, 6,
                  7, 8, 9};
    auto m2 = Mat{ 1, -1,  1,
                  -1,  1, -1,
                   1, -1,  1};
    auto product =  Mat{0, 3, 2,
                        5, 4, 7,
                        6, 9, 8};
    Y_EQUAL(m1 - m2, product);
}

void test_MatrixMatrixMultiplication()
{
    typedef Matrix<int, 2> Mat;
    auto m1 = Mat{1, 2,
                  3, 4};
    auto m2 = Mat{4, 3,
                  1, 2};
    auto product =  Mat{ 6,  7,
                        16, 17};
    Y_EQUAL(m1 * m2, product);
}

void test_MatrixVectorMultiplication()
{
    auto m = Matrix<int, 3>{1, 2, 3,
                            4, 5, 6,
                            7, 8, 9};
    auto v = Vector<int, 3>{1, 2, 3};
    auto product1 =  Vector<int, 3>{14, 32, 50};
    auto product2 =  Vector<int, 3>{30, 36, 42};
    Y_EQUAL(m * v, product1);
    Y_EQUAL(v * m, product2);
}

Y_SUBTEST("Geometry",
          test_Basics,
          test_Transpose,
          test_MatrixMatrixAddition,
          test_MatrixMatrixSubtraction,
          test_MatrixMatrixMultiplication,
          test_MatrixVectorMultiplication);
}
