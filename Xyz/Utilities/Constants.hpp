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
//#include "JEBMath/JEBMathDefinitions.hpp"

namespace Xyz
{
    constexpr size_t INVALID_INDEX = SIZE_MAX;
    constexpr double INVALID_DOUBLE = -DBL_MAX;
    constexpr double DOUBLE_TOLERANCE = 1e-9;
    constexpr float FLOAT_TOLERANCE = 1e-5;

    constexpr double PI_64 = 3.1415926535897932384626433832795;
    constexpr float PI_32 = 3.1415926535897932384626433832795;

    static inline bool isValid(double v)
    {
        return v != INVALID_DOUBLE;
    }

    static inline bool isValid(size_t v)
    {
        return v != INVALID_INDEX;
    }
}
