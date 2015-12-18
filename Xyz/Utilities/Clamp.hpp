//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include "Constants.hpp"

namespace Xyz {

    template <typename T>
    T getClamped(T value, T min, T max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    template <typename T>
    void clamp(T& value, T min, T max)
    {
        if (value < min)
            value = min;
        else if (value > max)
            value = max;
    }

}
