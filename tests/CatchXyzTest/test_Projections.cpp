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

TEST_CASE("test look_at")
{
    using V = Xyz::Vector4D;
    auto m = Xyz::make_look_at_matrix<double>({5, 2, 3},
                                              {1, 8, 3},
                                              {0, 0, 1});
    const auto result = m * V(1.5, 4.0, 3.0, 1.0);
    const auto expected = V(-std::sqrt(1 + 1.5 * 1.5),
                            0.0,
                            -std::sqrt(2 * 2 + 3 * 3),
                            1.0);
    CHECK(are_equal(result, expected, 1e-10));

    // Additional tests that make it more obvious what the transformation
    // actually does to the points.
    const auto m2 = Xyz::make_look_at_matrix<double>(
        {0, 0, -10},
        {0, 0, 0},
        {0, 1, 0});
    REQUIRE(m2 * V{-1, -1, 0, 1} == V{1, -1, -10, 1});
    REQUIRE(m2 * V{1, -1, 0, 1} == V{-1, -1, -10, 1});
    REQUIRE(m2 * V{-1, 1, -1, 1} == V{1, 1, -9, 1});
}

TEST_CASE("decompose view matrix")
{
    using V = Xyz::Vector3D;
    auto view = Xyz::make_look_at_matrix<double>(
        {5, 2, 3},
        {1, 8, 3},
        {0, 0, 1});
    const auto dec = Xyz::decompose_view_matrix(view);
    CHECK(are_equal(dec.forward, V{-4 / sqrt(16 + 36 + 0),
                                 6 / sqrt(16 + 36 + 0),
                                 0}));
    CHECK(are_equal(dec.up, V{0, 0, 1}));
    CHECK(are_equal(dec.right, V{6 / sqrt(16 + 36 + 0),
                                 4 / sqrt(16 + 36 + 0),
                                 0}));
    CHECK(are_equal(dec.position, V{5, 2, 3}));
}
