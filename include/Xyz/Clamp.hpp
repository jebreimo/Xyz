//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cmath>
#include "Constants.hpp"

namespace Xyz
{
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
    T& clamp(T& value, T min, T max)
    {
        if (value < min)
            value = min;
        else if (value > max)
            value = max;
        return value;
    }

    template <typename It, typename T>
    void scaleRange(It first, It last, T newMin, T newMax)
    {
        auto its = std::minmax_element(first, last);
        if (its.first == last)
            return;

        auto curMin = *its.first;
        auto curMax = *its.second;
        auto curRange = curMax - curMin;

        auto newRange = newMax - newMin;

        for (auto it = first; it != last; ++it)
            *it = newMin + (*it - curMin) * newRange / curRange;
    }

    template <typename It, typename T>
    void clampRange(It first, It last, T newMin, T newMax)
    {
        for (auto it = first; it != last; ++it)
            Xyz::clamp(*it, newMin, newMax);
    }
}
