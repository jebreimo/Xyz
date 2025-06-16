//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cfloat>
#include <type_traits>

namespace Xyz
{
    template <typename T>
    struct Constants
    {
        constexpr static double PI = 3.141592653589793;
    };

    template <>
    struct Constants<float>
    {
        constexpr static float PI = 3.14159265f;
    };

    template <typename T>
    struct Margin
    {
        static constexpr T default_margin()
        {
            if constexpr (std::is_floating_point_v<T>)
                return std::numeric_limits<T>::epsilon() * 100.0;
            else
                return T(0);
        }

        constexpr static T DEFAULT = default_margin();
    };
}
