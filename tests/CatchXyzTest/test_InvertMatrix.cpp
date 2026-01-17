//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-05.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/InvertMatrix.hpp"
#include <catch2/catch_test_macros.hpp>

template <typename T, unsigned N>
bool equals_identity_matrix(const Xyz::Matrix<T, N, N>& m)
{
    return are_equal(m, Xyz::make_identity_matrix<T, N>());
}

TEST_CASE("test invert2")
{
    Xyz::Matrix2D m{
        1, 4,
        2, 1
    };
    CHECK(equals_identity_matrix(m * invert(m)));
}

TEST_CASE("test invert3")
{
    Xyz::Matrix3D m{
        1, 4, 2,
        2, 1, 3,
        5, 7, 2
    };
    CHECK(equals_identity_matrix(m * invert(m)));
}

TEST_CASE("test invert4")
{
    Xyz::Matrix4D m{
        1, -2, 3, 2,
        2, 3, 1, -1,
        3, 7, 0, 3,
        -1, 4, 2, 1
    };
    CHECK(equals_identity_matrix(m * invert(m)));
}

TEST_CASE("test invert5")
{
    Xyz::Matrix<double, 5, 5> m{
        1, -2, 3, 2, 5,
        2, 3, 1, -1, 7,
        3, 7, 0, 3, 3,
        -1, 4, 2, 1, 2,
        5, 0, -2, 4, 1
    };
    CHECK(equals_identity_matrix(m * invert(m)));
}
