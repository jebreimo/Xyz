//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-25.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Matrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;
using Xyz::RowCol;

TEST_CASE("Matrix: test_basics")
{
    auto mat = Xyz::Matrix3D{
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    CHECK(mat[RowCol(0, 0)] == 1.0);
    CHECK(mat[RowCol(0, 1)] == 0.0);
    CHECK(mat[RowCol(1, 1)] == 1.0);
}

TEST_CASE("Matrix: test_transpose")
{
    auto mat1 = Xyz::Matrix4D{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 0, 1,
        2, 3, 4, 5
    };
    auto mat1t = Xyz::Matrix4D{
        0, 4, 8, 2,
        1, 5, 9, 3,
        2, 6, 0, 4,
        3, 7, 1, 5
    };
    CHECK(transpose(mat1) == mat1t);
    transpose_inplace(mat1t);
    CHECK(mat1 == mat1t);
}

TEST_CASE("Matrix: test_matrix_matrix_addition")
{
    auto m1 = Xyz::Matrix3I{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    auto m2 = Xyz::Matrix3I{
        1, -1, 1,
        -1, 1, -1,
        1, -1, 1
    };
    auto product = Xyz::Matrix3I{
        2, 1, 4,
        3, 6, 5,
        8, 7, 10
    };
    CHECK(m1 + m2 == product);
}

TEST_CASE("Matrix: test_matrix_matrix_subtraction")
{
    auto m1 = Xyz::Matrix3I{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    auto m2 = Xyz::Matrix3I{
        1, -1, 1,
        -1, 1, -1,
        1, -1, 1
    };
    auto product = Xyz::Matrix3I{
        0, 3, 2,
        5, 4, 7,
        6, 9, 8
    };
    CHECK(m1 - m2 == product);
}

TEST_CASE("Matrix: test_matrix_matrix_multiplication")
{
    auto m1 = Xyz::Matrix2I{
        1, 2,
        3, 4
    };
    auto m2 = Xyz::Matrix2I{
        4, 3,
        1, 2
    };
    auto product = Xyz::Matrix2I{
        6, 7,
        16, 17
    };
    CHECK(m1 * m2 == product);
}

TEST_CASE("Matrix: test_matrix_vector_multiplication")
{
    auto m = Xyz::Matrix3I{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    auto v = Xyz::Vector3I{1, 2, 3};
    auto product1 = Xyz::Vector3I{14, 32, 50};
    auto product2 = Xyz::Vector3I{30, 36, 42};
    CHECK(m * v == product1);
    CHECK(v * m == product2);
}

TEST_CASE("Matrix: test_make_submatrix")
{
    Xyz::Matrix4I m{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 0, 1, 2,
        3, 4, 5, 6
    };
    auto s = make_submatrix<3, 3>(m, {3, 2});
    Xyz::Matrix3I e{
        5, 6, 3,
        3, 4, 1,
        7, 8, 5
    };
    CHECK(s == e);
}

TEST_CASE("Matrix: test_identity")
{
    auto m1 = Xyz::make_identity_matrix<int, 4>();
    CHECK(m1[RowCol(0, 0)] == 1);
    CHECK(m1[RowCol(0, 1)] == 0);
}

TEST_CASE("Matrix: outer product")
{
    auto u = Xyz::Vector3F{1, 2, 3};
    auto v = Xyz::Vector3F{4, 5, 6};
    auto result = Xyz::outer_product(u, v);
    Xyz::Matrix3F expected{
        4, 5, 6,
        8, 10, 12,
        12, 15, 18
    };
    CHECK(result == expected);
}

TEST_CASE("Matrix: get and set row")
{
    Xyz::Matrix3I m{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    auto row = get_row(m, 1);
    CHECK(row == Xyz::Vector3I{4, 5, 6});
    set_row(m, 1, Xyz::Vector3I{10, 11, 12});
    CHECK(m[RowCol(1, 0)] == 10);
    CHECK(m[RowCol(1, 1)] == 11);
    CHECK(m[RowCol(1, 2)] == 12);
}

TEST_CASE("Matrix: get and set column")
{
    Xyz::Matrix3I m{
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };
    auto col = get_col(m, 1);
    CHECK(col == Xyz::Vector3I{2, 5, 8});
    set_col(m, 1, Xyz::Vector3I{10, 11, 12});
    CHECK(m[RowCol(0, 1)] == 10);
    CHECK(m[RowCol(1, 1)] == 11);
    CHECK(m[RowCol(2, 1)] == 12);
}
