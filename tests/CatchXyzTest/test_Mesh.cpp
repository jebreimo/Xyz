//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-06-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Mesh.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Cube mesh")
{
    Xyz::Mesh<float> cube(
        {
            {-1, -1, -1},
            {1, -1, -1},
            {-1, -1, 1},
            {1, -1, 1},
            {1, 1, -1},
            {-1, 1, -1},
            {1, 1, 1},
            {-1, 1, 1}},
        {
            {0, 1, 2},
            {1, 3, 2},
            {1, 4, 3},
            {4, 6, 3},
            {4, 5, 6},
            {5, 7, 6},
            {5, 0, 7},
            {0, 2, 7},
            {0, 5, 1},
            {5, 4, 1},
            {2, 3, 7},
            {3, 6, 7}});
    REQUIRE(cube.normal(cube.faces()[0]) == Xyz::Vector3F{0, -1, 0});
}
