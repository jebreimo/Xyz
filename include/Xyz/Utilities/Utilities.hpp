//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
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

    template <typename T>
    constexpr auto toRadians(T degrees)
    {
        return degrees * Constants<T>::PI / 180;
    }

    template <typename T>
    constexpr double toDegrees(T radians)
    {
        return radians * 180 / Constants<T>::PI;
    }

    inline int modulo(int dividend, int divisor)
    {
        if (dividend >= 0)
            return dividend % divisor;
        else
            return divisor - (-dividend % divisor);
    }
}
