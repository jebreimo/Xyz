//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-31.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/MatrixDeterminant.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test get determinant3")
{
    const Xyz::Matrix3I m{
        1, 4, 3,
        2, 1, 5,
        3, 2, 1
    };
    CHECK(get_determinant(m) == 46);
}

TEST_CASE("test get determinant4")
{
    const Xyz::Matrix4I m{
        1, -2, 3, 2,
        2, 3, 1, -1,
        3, 3, 3, 3,
        -1, 4, 2, 1
    };
    CHECK(get_determinant(m) == -141);
}
