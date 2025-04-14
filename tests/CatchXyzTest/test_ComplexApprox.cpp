//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/ComplexApprox.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ComplexApprox comparisons")
{
    Xyz::ComplexApprox<double> a({1, 1}, {0.01, 0.01});

    REQUIRE(a == std::complex<double>(0.991, 1));
    REQUIRE(a == std::complex<double>(1.009, 1));
    REQUIRE(a == std::complex<double>(1, 0.991));
    REQUIRE(a == std::complex<double>(1, 1.009));

    REQUIRE_FALSE(a == std::complex<double>(0.989, 1));
    REQUIRE_FALSE(a == std::complex<double>(1.011, 1));
    REQUIRE_FALSE(a == std::complex<double>(1, 0.989));
    REQUIRE_FALSE(a == std::complex<double>(1, 1.011));
}
