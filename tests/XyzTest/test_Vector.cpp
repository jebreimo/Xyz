//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Vector.hpp"

#include <cmath>
#include <Ytest/Ytest.hpp>
#include "Xyz/Utilities.hpp"
#include "YtestUtilities.hpp"

namespace
{
    using namespace Xyz;

    void test_basics_2d()
    {
        auto u = make_vector2(2.0, 1.0);
        auto v = make_vector2(3.0, 4.0);
        Y_EQUAL(u[0], 2);
        Y_EQUAL(u[1], 1);
        Y_EQUAL(v[0], 3);
        Y_EQUAL(v[1], 4);
        Y_EQUAL(u * v, make_vector2<double>(6, 4));
        Y_EQUAL(u + v, make_vector2<double>(5, 5));
        Y_EQUAL(u - v, make_vector2<double>(-1, -3));
        Y_EQUAL(u * 3., make_vector2<double>(6, 3));
        Y_EQUAL(3. * u, make_vector2<double>(6, 3));
        Y_EQUAL(u / 3., make_vector2<double>(2.0 / 3, 1.0 / 3));
        Y_EQUAL(6. / u, make_vector2<double>(3, 6));
        Y_EQUAL(u += v, make_vector2<double>(5, 5));
        Y_EQUAL(u -= v, make_vector2<double>(2, 1));
        Y_EQUAL(u *= 3., make_vector2<double>(6, 3));
        Y_EQUAL(u /= 2., make_vector2<double>(3.0, 1.5));
        Y_EQUAL(u *= v, make_vector2<double>(9, 6));
        Y_EQUAL(u *= 2., make_vector2<double>(18, 12));
        Y_EQUAL(u /= v, make_vector2<double>(6, 3));
        Y_EQUAL(dot(u, v), 30);
        Y_EQUIVALENT(get_length(v), 5, 1e-10);
    }

    void test_basics_4d()
    {
        auto u = make_vector4(2.0, 1.0, 4.0, 3.0);
        auto v = make_vector4(3.0, 4.0, -1.0, -2.0);
        Y_EQUAL(u[0], 2);
        Y_EQUAL(u[1], 1);
        Y_EQUAL(u[2], 4);
        Y_EQUAL(u[3], 3);
        Y_EQUAL(v[0], 3);
        Y_EQUAL(v[1], 4);
        Y_EQUAL(v[2], -1);
        Y_EQUAL(v[3], -2);
        Y_EQUAL(u * v, make_vector4<double>(6, 4, -4, -6));
        Y_EQUAL(u + v, make_vector4<double>(5, 5, 3, 1));
        Y_EQUAL(u - v, make_vector4<double>(-1, -3, 5, 5));
        Y_EQUAL(u * 3., make_vector4<double>(6, 3, 12, 9));
        Y_EQUAL(3. * u, make_vector4<double>(6, 3, 12, 9));
        Y_EQUAL(u / 3., make_vector4<double>(2.0 / 3, 1.0 / 3, 4.0 / 3, 1.0));
        Y_EQUAL(12. / u, make_vector4<double>(6, 12, 3, 4));
        Y_EQUAL(u += v, make_vector4<double>(5, 5, 3, 1));
        Y_EQUAL(u -= v, make_vector4<double>(2, 1, 4, 3));
        Y_EQUAL(u *= 3., make_vector4<double>(6, 3, 12, 9));
        Y_EQUAL(u /= 2., make_vector4<double>(3.0, 1.5, 6.0, 4.5));
        Y_EQUAL(u *= v, make_vector4<double>(9, 6, -6, -9));
        Y_EQUAL(u *= 2., make_vector4<double>(18, 12, -12, -18));
        Y_EQUAL(u /= v, make_vector4<double>(6, 3, 12, 9));
        Y_EQUAL(dot(u, v), 18 + 12 - 12 - 18);
        Y_EQUIVALENT(get_length(v), sqrt(9 + 16 + 1 + 4), 1e-10);
    }

    void test_constructors()
    {
        auto u = make_vector4(1, 2, 0, 1);
        Vector<double, 4> v = vector_cast<double>(u);
        Y_EQUAL(v[0], 1);
        Y_EQUAL(v[1], 2);
        Y_EQUAL(v[2], 0);
        Y_EQUAL(v[3], 1);
    }

    void test_cross()
    {
        auto u = make_vector3(1, 2, 3);
        auto v = make_vector3(0, 1, 2);
        Y_EQUAL(cross(u, v), make_vector3(1, -2, 1));
    }

    void test_get_ccw_angle()
    {
        Y_EQUIVALENT(get_ccw_angle(make_vector2(6, 6), make_vector2(4, -4)),
                     3 * Constants<double>::PI / 2, 1e-10);
    }

    void test_rotated()
    {
        auto sqrt2 = std::sqrt(2);
        Y_EQUIVALENT(rotate(make_vector2<double>(100, 0), to_radians(30)),
                     make_vector2(50 * std::sqrt(3), 50.0),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2<double>(100, 0), to_radians(45)),
                     make_vector2(100 / sqrt2, 100 / sqrt2),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2<double>(100, 0), to_radians(60)),
                     make_vector2(50.0, 50 * std::sqrt(3)),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2<double>(0, 100), to_radians(-60)),
                     make_vector2(50 * std::sqrt(3), 50.0),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2<double>(0, 100), to_radians(-45)),
                     make_vector2(100 / sqrt2, 100 / sqrt2),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2<double>(0, 100), to_radians(-30)),
                     make_vector2(50.0, 50 * std::sqrt(3)),
                     1e-10);

        Y_EQUIVALENT(rotate(make_vector2(1 / sqrt2, 1 / sqrt2), to_radians(45)),
                     make_vector2<double>(0, 1),
                     1e-10);
        Y_EQUIVALENT(rotate(make_vector2(1 / sqrt2, 1 / sqrt2), to_radians(135)),
                     make_vector2<double>(-1, 0),
                     1e-10);
    }

    void test_types()
    {
        auto u = make_vector2(1, 2);
        Y_EQUAL(typeid(typename decltype(u)::ValueType).name(),
                typeid(int).name());
        auto v = make_vector2(2.0, 4.0);
        Y_EQUAL(typeid(typename decltype(v)::ValueType).name(),
                typeid(double).name());
        auto w = vector_cast<double>(u) + v;
        Y_EQUAL(typeid(typename decltype(w)::ValueType).name(),
                typeid(double).name());
    }

    Y_SUBTEST("Fundamentals",
              test_basics_2d,
              test_basics_4d,
              test_constructors,
              test_cross,
              test_get_ccw_angle,
              test_rotated,
              test_types);
}
