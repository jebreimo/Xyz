//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cmath>
#include <concepts>
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
     * @brief Returns the @a radians as an angle between 0 and 2*π.
     */
    template <std::floating_point T>
    constexpr T to_principal_angle(T radians)
    {
        constexpr auto pi2 = T(2) * Constants<T>::PI;
        if (0 <= radians && radians < pi2)
            return radians;
        auto result = fmod(radians, pi2);
        return result >= 0 ? result : result + pi2;
    }

    /**
     * @brief Returns the @a radians as an angle between -π and π.
     */
    template <std::floating_point T>
    constexpr T to_signed_principal_angle(T radians)
    {
        constexpr auto pi = Constants<T>::PI;
        if (-pi <= radians && radians < pi)
            return radians;
        auto result = fmod(radians + pi, T(2) * pi);
        return result >= 0 ? result - pi : result + pi;
    }
}
