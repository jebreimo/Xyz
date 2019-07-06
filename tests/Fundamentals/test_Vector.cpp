//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Vector.hpp"

#include <cmath>
#include "Ytest/Ytest.hpp"
#include "Xyz/Utilities/Utilities.hpp"

namespace {

    using namespace Xyz;

    void test_Basics2D()
    {
        auto u = makeVector(2.0, 1.0);
        auto v = makeVector(3.0, 4.0);
        Y_EQUAL(u[0], 2);
        Y_EQUAL(u[1], 1);
        Y_EQUAL(v[0], 3);
        Y_EQUAL(v[1], 4);
        Y_EQUAL(mul(u, v), makeVector<double>(6, 4));
        Y_EQUAL(u + v, makeVector<double>(5, 5));
        Y_EQUAL(u - v, makeVector<double>(-1, -3));
        Y_EQUAL(u * 3, makeVector<double>(6, 3));
        Y_EQUAL(3 * u, makeVector<double>(6, 3));
        Y_EQUAL(u / 3, makeVector<double>(2.0 / 3, 1.0 / 3));
        Y_EQUAL(div(6, u), makeVector<double>(3, 6));
        Y_EQUAL(u += v, makeVector<double>(5, 5));
        Y_EQUAL(u -= v, makeVector<double>(2, 1));
        Y_EQUAL(u *= 3, makeVector<double>(6, 3));
        Y_EQUAL(u /= 2, makeVector<double>(3.0, 1.5));
        Y_EQUAL(assignMul(u, v), makeVector<double>(9, 6));
        Y_EQUAL(u *= 2, makeVector<double>(18, 12));
        Y_EQUAL(assignDiv(u, v), makeVector<double>(6, 3));
        Y_EQUAL(u * v, 30);
        Y_EQUIVALENT(length(v), 5, 1e-10);
    }

    void test_Basics4D()
    {
        auto u = makeVector(2.0, 1.0, 4.0, 3.0);
        auto v = makeVector(3.0, 4.0, -1.0, -2.0);
        Y_EQUAL(u[0], 2);
        Y_EQUAL(u[1], 1);
        Y_EQUAL(u[2], 4);
        Y_EQUAL(u[3], 3);
        Y_EQUAL(v[0], 3);
        Y_EQUAL(v[1], 4);
        Y_EQUAL(v[2], -1);
        Y_EQUAL(v[3], -2);
        Y_EQUAL(mul(u, v), makeVector<double>(6, 4, -4, -6));
        Y_EQUAL(u + v, makeVector<double>(5, 5, 3, 1));
        Y_EQUAL(u - v, makeVector<double>(-1, -3, 5, 5));
        Y_EQUAL(u * 3, makeVector<double>(6, 3, 12, 9));
        Y_EQUAL(3 * u, makeVector<double>(6, 3, 12, 9));
        Y_EQUAL(u / 3, makeVector<double>(2.0 / 3, 1.0 / 3, 4.0 / 3, 1.0));
        Y_EQUAL(div(12, u), makeVector<double>(6, 12, 3, 4));
        Y_EQUAL(u += v, makeVector<double>(5, 5, 3, 1));
        Y_EQUAL(u -= v, makeVector<double>(2, 1, 4, 3));
        Y_EQUAL(u *= 3, makeVector<double>(6, 3, 12, 9));
        Y_EQUAL(u /= 2, makeVector<double>(3.0, 1.5, 6.0, 4.5));
        Y_EQUAL(assignMul(u, v), makeVector<double>(9, 6, -6, -9));
        Y_EQUAL(u *= 2, makeVector<double>(18, 12, -12, -18));
        Y_EQUAL(assignDiv(u, v), makeVector<double>(6, 3, 12, 9));
        Y_EQUAL(u * v, 18 + 12 - 12 - 18);
        Y_EQUIVALENT(length(v), sqrt(9 + 16 + 1 + 4), 1e-10);
    }

    void test_Constructors()
    {
        auto u = makeVector(1, 2, 0, 1);
        Vector<double, 4> v = u;
        Y_EQUAL(v[0], 1);
        Y_EQUAL(v[1], 2);
        Y_EQUAL(v[2], 0);
        Y_EQUAL(v[3], 1);
    }

    void test_cross()
    {
        auto u = makeVector(1, 2, 3);
        auto v = makeVector(0, 1, 2);
        Y_EQUAL(cross(u, v), makeVector(1, -2, 1));
    }

    void test_getCounterclockwiseAngle()
    {
        Y_EQUIVALENT(counterclockwiseAngle(makeVector(6, 6),
                                           makeVector(4, -4)),
                     3 * PI_64 / 2, 1e-10);
    }

    void test_rotated()
    {
        auto sqrt2 = std::sqrt(2);
        Y_ASSERT(equivalent(rotated(makeVector<double>(100, 0), toRadians(30)),
                            makeVector(50 * std::sqrt(3), 50.0), 1e-12));
        Y_ASSERT(equivalent(rotated(makeVector<double>(100, 0), toRadians(45)),
                            makeVector(100 / sqrt2, 100 / sqrt2), 1e-12));
        Y_ASSERT(equivalent(rotated(makeVector<double>(100, 0), toRadians(60)),
                            makeVector(50.0, 50 * std::sqrt(3)), 1e-12));
        Y_ASSERT(equivalent(rotated(makeVector<double>(0, 100), toRadians(-60)),
                            makeVector(50 * std::sqrt(3), 50.0), 1e-12));
        Y_ASSERT(equivalent(rotated(makeVector<double>(0, 100), toRadians(-45)),
                            makeVector(100 / sqrt2, 100 / sqrt2), 1e-12));
        Y_ASSERT(equivalent(rotated(makeVector<double>(0, 100), toRadians(-30)),
                            makeVector(50.0, 50 * std::sqrt(3)), 1e-12));

        Y_ASSERT(equivalent(
                rotated(makeVector(1 / sqrt2, 1 / sqrt2), toRadians(45)),
                makeVector<double>(0, 1), 1e-12));
        Y_ASSERT(equivalent(
                rotated(makeVector(1 / sqrt2, 1 / sqrt2), toRadians(135)),
                makeVector<double>(-1, 0), 1e-12));
    }

    void test_Types()
    {
        auto u = makeVector(1, 2);
        Y_EQUAL(typeid(typename decltype(u)::ValueType).name(),
                typeid(int).name());
        auto v = makeVector(2.0, 4.0);
        Y_EQUAL(typeid(typename decltype(v)::ValueType).name(),
                typeid(double).name());
        auto w = makeVector<double>(u) + v;
        Y_EQUAL(typeid(typename decltype(w)::ValueType).name(),
                typeid(double).name());
    }

    Y_SUBTEST("Fundamentals",
              test_Basics2D,
              test_Basics4D,
              test_Constructors,
              test_cross,
              test_getCounterclockwiseAngle,
              test_rotated,
              test_Types);
}
