//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cfloat>
#include <climits>
#include <cstdint>

namespace Xyz
{
    template <typename T>
    struct Constants
    {
        constexpr static double PI = 3.141592653589793;
        constexpr static double DEFAULT_MARGIN = DBL_EPSILON * 100.0;
    };

    template <>
    struct Constants<float>
    {
        constexpr static float PI = 3.14159265f;
        constexpr static float DEFAULT_MARGIN = FLT_EPSILON * 100.0;
    };
}
