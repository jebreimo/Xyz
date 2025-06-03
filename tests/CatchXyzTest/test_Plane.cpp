//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Plane.hpp>

#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Plane: projection matrix on")
{
    using namespace Xyz;
    using Catch::Matchers::WithinAbs;

    Plane<double, 3> plane{{2, -1, 0}, {0, 1, 1}};
    auto m = make_projection_matrix(plane);
    using V = Vector4D;
    CHECK(are_equal(m * V(2, -1, 0, 1), V(2, -1, 0, 1), 1e-10));
    CHECK(are_equal(m * V(5, 2, 3, 1), V(5, -1, 0, 1), 1e-10));
}
