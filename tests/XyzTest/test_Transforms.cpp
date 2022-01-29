//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace {
    void test_rotate_z()
    {
        auto trans = Xyz::rotate_z(Xyz::to_radians(90.0));
        auto v = trans * Xyz::make_vector4<double>(1, 1, 1, 1);
        Y_EQUIVALENT(v, Xyz::make_vector4<double>(-1, 1, 1, 1), 1e-10);
    }

    void test_translate4()
    {
        auto trans = Xyz::translate4(1.0, 2.0, 3.0);
        auto v = trans * Xyz::make_vector4<double>(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::make_vector4<double>(2, 3, 4, 1));
    }

    void test_rotate_z_and_transposed_translate4()
    {
        auto trans = multiply_transposed(
                Xyz::rotate_z(Xyz::to_radians(90.0)),
                Xyz::transposed_translate4<double>(1, 2, 3));
        auto v = trans * Xyz::make_vector4<double>(1, 1, 1, 1);
        Y_EQUAL(v, Xyz::make_vector4<double>(-3, 2, 4, 1));
    }

    Y_SUBTEST("Fundamentals",
              test_rotate_z,
              test_translate4,
              test_rotate_z_and_transposed_translate4);

}
