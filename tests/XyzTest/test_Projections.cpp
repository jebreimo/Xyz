//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace
{
    void test_perspective_projection()
    {
        auto m = Xyz::make_frustum_matrix<double>(-1, 1, -1, 1, 9, 11);
        auto v = Xyz::make_vector4<double>(-1.0, -1.0, -9.0, 1.0);
        auto w = m * v;
        Y_EQUAL(w, Xyz::make_vector4<double>(-9, -9, -9, 9));
    }

    void test_look_at()
    {
        auto m = Xyz::make_look_at_matrix(Xyz::make_vector3<double>(5, 2, 3),
                                       Xyz::make_vector3<double>(1, 8, 3),
                                       Xyz::make_vector3<double>(0, 0, 1));
        const auto result = m * Xyz::make_vector4(1.5, 4.0, 3.0, 1.0);
        const auto expected = Xyz::make_vector4(-std::sqrt(1 + 1.5 * 1.5),
                                                0.0,
                                                -std::sqrt(2 * 2 + 3 * 3),
                                                1.0);
        Y_EQUIVALENT(result, expected, 1e-10);
    }

    Y_SUBTEST("Fundamentals",
              test_perspective_projection,
              test_look_at);
}
