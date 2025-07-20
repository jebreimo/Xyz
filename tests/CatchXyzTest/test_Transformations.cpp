//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_rotate z")
{
    auto trans = Xyz::affine::rotate_z(Xyz::to_radians(90.0));
    auto v = trans * Xyz::Vector4D(1, 1, 1, 1);
    CHECK(are_equal(v, Xyz::Vector4D(-1, 1, 1, 1), 1e-10));
}

TEST_CASE("test translate4")
{
    auto trans = Xyz::affine::translate3(1.0, 2.0, 3.0);
    auto v = trans * Xyz::Vector4D(1, 1, 1, 1);
    CHECK(are_equal(v, Xyz::Vector4D(2, 3, 4, 1)));
}

TEST_CASE("test_rotate_z_and_transposed translate4")
{
    auto trans = multiply_transposed(
            Xyz::affine::rotate_z(Xyz::to_radians(90.0)),
            Xyz::affine::transposed_translate4<double>(1, 2, 3));
    auto v = trans * Xyz::Vector4D(1, 1, 1, 1);
    CHECK(are_equal(v, Xyz::Vector4D(-3, 2, 4, 1)));
}
