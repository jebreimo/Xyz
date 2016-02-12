//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include <cstdlib>
#include "Constants.hpp"

namespace Xyz {

    template <typename T>
    bool equal(T a, T b, T tolerance)
    {
        return std::abs(a - b) <= tolerance;
    }

    template <>
    inline bool equal(double a, double b, double tolerance)
    {
        return std::fabs(a - b) <= tolerance;
    }

    template <>
    inline bool equal(float a, float b, float tolerance)
    {
        return std::fabs(a - b) <= tolerance;
    }

    template <typename T>
    bool equal(T a, T b)
    {
        return a == b;
    }

    template <>
    inline bool equal(double a, double b)
    {
        return equal(a, b, DoubleTolerance);
    }

    template <typename T>
    bool less(T a, T b, T tolerance)
    {
        return a < b - tolerance;
    }

    template <typename T>
    bool lessOrEqual(T a, T b, T tolerance)
    {
        return a <= b + tolerance;
    }

    template <typename T>
    bool greater(T a, T b, T tolerance)
    {
        return a > b + tolerance;
    }

    template <typename T>
    bool greaterOrEqual(T a, T b, T tolerance)
    {
        return a >= b - tolerance;
    }

    template <typename T>
    bool between(T from, T value, T to, T tolerance)
    {
        return greater(value, from, tolerance) &&
               less(value, to, tolerance);
    }

    template <typename T>
    bool inRange(T from, T value, T to, T tolerance)
    {
        return greaterOrEqual(value, from, tolerance) &&
               lessOrEqual(value, to, tolerance);
    }

}
