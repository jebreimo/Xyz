//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cmath>
#include "Constants.hpp"

namespace Xyz
{
    template <typename T>
    constexpr auto to_radians(T degrees)
    {
        return degrees * Constants<T>::PI / 180;
    }

    template <typename T>
    constexpr auto to_degrees(T radians)
    {
        return radians * 180 / Constants<T>::PI;
    }

    /**
     * @brief Returns the @a radians as an angle between 0 and 2*PI.
     */
    template <typename T>
    constexpr auto to_principal_angle(T radians)
    {
        auto result = fmod(radians, T(2) * Constants<T>::PI);
        return result >= 0 ? result : result + T(2) * Constants<T>::PI;
    }
}
