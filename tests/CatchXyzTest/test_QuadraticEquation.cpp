//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/ComplexApprox.hpp>
#include <Xyz/QuadraticEquation.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Find real solution to quadratic equation")
{
    auto solution = Xyz::solve_real_quadratic_equation(4, 18, -70);
    REQUIRE(bool(solution));
    auto [lo, hi] = *solution;
    REQUIRE(lo == -7);
    REQUIRE(hi == 2.5);
}

TEST_CASE("Equation without real solution")
{
    auto solution = Xyz::solve_real_quadratic_equation(1, 0, 1);
    REQUIRE_FALSE(bool(solution));
}

TEST_CASE("Equation with complex solution")
{
    auto [lo, hi] = Xyz::solve_complex_quadratic_equation<double>(1, 0, 1);
    REQUIRE(lo == Xyz::ComplexApprox<double>({0, -1}));
    REQUIRE(hi == Xyz::ComplexApprox<double>({0, 1}));
}
