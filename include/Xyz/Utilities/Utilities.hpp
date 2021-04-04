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
    template <typename T>
    constexpr auto toRadians(T degrees)
    {
        return degrees * Constants<T>::PI / 180;
    }

    template <typename T>
    constexpr auto toDegrees(T radians)
    {
        return radians * 180 / Constants<T>::PI;
    }

    template <typename T>
    constexpr auto toPrincipalAngle(T radians)
    {
        auto result = fmod(radians, 2 * Constants<T>::PI);
        return result >= 0 ? result : result + 2 * Constants<T>::PI;
    }
}
