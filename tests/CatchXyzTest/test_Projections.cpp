//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_perspective_projection")
{
    auto m = Xyz::make_frustum_matrix<double>(-1, 1, -1, 1, 9, 11);
    auto v = Xyz::Vector4D(-1.0, -1.0, -9.0, 1.0);
    auto w = m * v;
    CHECK(w == Xyz::Vector4D(-9, -9, -9, 9));
}

TEST_CASE("test_look_at")
{
    auto m = Xyz::make_look_at_matrix(Xyz::Vector3D(5, 2, 3),
                                      Xyz::Vector3D(1, 8, 3),
                                      Xyz::Vector3D(0, 0, 1));
    const auto result = m * Xyz::Vector4D(1.5, 4.0, 3.0, 1.0);
    const auto expected = Xyz::Vector4D(-std::sqrt(1 + 1.5 * 1.5),
                                        0.0,
                                        -std::sqrt(2 * 2 + 3 * 3),
                                        1.0);
    CHECK(are_equal(result, expected, 1e-10));
}
