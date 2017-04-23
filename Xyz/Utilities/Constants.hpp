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

namespace Xyz {

    constexpr size_t InvalidIndex = SIZE_MAX;
    constexpr double InvalidDouble = -DBL_MAX;
    constexpr double DoubleTolerance = 1e-9;
    constexpr float FloatTolerance = 1e-5;

    constexpr double Pi = 3.1415926535897932384626433832795;

    static inline bool isValid(double v)
    {
        return v != InvalidDouble;
    }

    static inline bool isValid(size_t v)
    {
        return v != InvalidIndex;
    }

}
