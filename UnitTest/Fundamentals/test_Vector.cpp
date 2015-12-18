//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Vector.hpp"

#include <cmath>
#include "../../Externals/Ytest/Ytest.hpp"
#include "../../Xyz/Utilities/Utilities.hpp"

namespace {

using namespace Xyz;

void test_Basics2D()
{
    auto u = vector2(2.0, 1.0);
    auto v = vector2(3, 4);
    Y_EQUAL(u[0], 2);
    Y_EQUAL(u[1], 1);
    Y_EQUAL(v[0], 3);
    Y_EQUAL(v[1], 4);
    Y_EQUAL(mul(u, v), vector2(6, 4));
    Y_EQUAL(u + v, vector2(5, 5));
    Y_EQUAL(u - v, vector2(-1, -3));
    Y_EQUAL(u * 3, vector2(6, 3));
    Y_EQUAL(3 * u, vector2(6, 3));
    Y_EQUAL(u / 3, vector2(2.0 / 3, 1.0 / 3));
    Y_EQUAL(div(6, u), vector2(3, 6));
    Y_EQUAL(u += v, vector2(5, 5));
    Y_EQUAL(u -= v, vector2(2, 1));
    Y_EQUAL(u *= 3, vector2(6, 3));
    Y_EQUAL(u /= 2, vector2(3.0, 1.5));
    Y_EQUAL(assignMul(u, v), vector2(9, 6));
    Y_EQUAL(u *= 2, vector2(18, 12));
    Y_EQUAL(assignDiv(u, v), vector2(6, 3));
    Y_EQUAL(u * v, 30);
    Y_EQUIVALENT(getLength(v), 5, 1e-10);
}

void test_Basics4D()
{
    auto u = vector4(2.0, 1.0, 4.0, 3.0);
    auto v = vector4(3, 4, -1, -2);
    Y_EQUAL(u[0], 2);
    Y_EQUAL(u[1], 1);
    Y_EQUAL(u[2], 4);
    Y_EQUAL(u[3], 3);
    Y_EQUAL(v[0], 3);
    Y_EQUAL(v[1], 4);
    Y_EQUAL(v[2], -1);
    Y_EQUAL(v[3], -2);
    Y_EQUAL(mul(u, v), vector4(6, 4, -4, -6));
    Y_EQUAL(u + v, vector4(5, 5, 3, 1));
    Y_EQUAL(u - v, vector4(-1, -3, 5, 5));
    Y_EQUAL(u * 3, vector4(6, 3, 12, 9));
    Y_EQUAL(3 * u, vector4(6, 3, 12, 9));
    Y_EQUAL(u / 3, vector4(2.0 / 3, 1.0 / 3, 4.0 / 3, 1.0));
    Y_EQUAL(div(12, u), vector4(6, 12, 3, 4));
    Y_EQUAL(u += v, vector4(5, 5, 3, 1));
    Y_EQUAL(u -= v, vector4(2, 1, 4, 3));
    Y_EQUAL(u *= 3, vector4(6, 3, 12, 9));
    Y_EQUAL(u /= 2, vector4(3.0, 1.5, 6.0, 4.5));
    Y_EQUAL(assignMul(u, v), vector4(9, 6, -6, -9));
    Y_EQUAL(u *= 2, vector4(18, 12, -12, -18));
    Y_EQUAL(assignDiv(u, v), vector4(6, 3, 12, 9));
    Y_EQUAL(u * v, 18 + 12 - 12 - 18);
    Y_EQUIVALENT(getLength(v), sqrt(9 + 16 + 1 + 4), 1e-10);
}

void test_Cross()
{
    auto u = vector3(1, 2, 3);
    auto v = vector3(0, 1, 2);
    Y_EQUAL(cross(u, v), vector3(1, -2, 1));
}

void test_Rotate()
{
    auto sqrt2 = std::sqrt(2);
    Y_ASSERT(areEquivalent(getRotated(vector2(100, 0), toRadians(30)),
                           vector2(50 * std::sqrt(3), 50.0), 1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(100, 0), toRadians(45)),
                           vector2(100 / sqrt2, 100 / sqrt2),
                           1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(100, 0), toRadians(60)),
                           vector2(50.0, 50 * std::sqrt(3)), 1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(0, 100), toRadians(-60)),
                           vector2(50 * std::sqrt(3), 50.0), 1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(0, 100), toRadians(-45)),
                           vector2(100 / sqrt2, 100 / sqrt2),
                           1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(0, 100), toRadians(-30)),
                           vector2(50.0, 50 * std::sqrt(3)), 1e-12));

    Y_ASSERT(areEquivalent(getRotated(vector2(1 / sqrt2, 1 / sqrt2),
                                      toRadians(45)),
                           vector2(0, 1),
                           1e-12));
    Y_ASSERT(areEquivalent(getRotated(vector2(1 / sqrt2, 1 / sqrt2),
                                      toRadians(135)),
                           vector2(-1, 0),
                           1e-12));
}

void test_Types()
{
    auto u = vector2(1, 2);
    Y_EQUAL(typeid(typename decltype(u)::ValueType).name(),
            typeid(int).name());
    auto v = vector2(2.0, 4.0);
    Y_EQUAL(typeid(typename decltype(v)::ValueType).name(),
            typeid(double).name());
    auto w = u + v;
    Y_EQUAL(typeid(typename decltype(w)::ValueType).name(),
            typeid(double).name());
}

void test_Constructors()
{
    auto u = vector2(1, 2);
    Vector<double, 4> v = u;
    Y_EQUAL(v[0], 1);
    Y_EQUAL(v[1], 2);
    Y_EQUAL(v[2], 0);
    Y_EQUAL(v[3], 0);
}

Y_SUBTEST("Geometry",
          test_Basics2D,
          test_Basics4D,
          test_Cross,
          test_Rotate,
          test_Types,
          test_Constructors);
}
