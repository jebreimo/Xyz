//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include "Xyz/Constants.hpp"

namespace Xyz
{
    template<typename T>
    T getSign(T value)
    {
        if (value > 0)
            return T(1);
        else if (value < 0)
            return T(-1);
        else
            return T(0);
    }

    constexpr double toRadians(double degrees)
    {
        return degrees * Pi / 180;
    }

    constexpr double toDegrees(double radians)
    {
        return radians * 180 / Pi;
    }

    inline int modulo(int dividend, int divisor)
    {
        if (dividend >= 0)
            return dividend % divisor;
        else
            return divisor - (-dividend % divisor);
    }
}
