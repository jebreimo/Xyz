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
    T get_clamped(T value, T min, T max)
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
    void scale_range(It first, It last, T new_min, T new_max)
    {
        auto its = std::minmax_element(first, last);
        if (its.first == last)
            return;

        auto cur_min = *its.first;
        auto cur_max = *its.second;
        auto cur_range = cur_max - cur_min;

        auto new_range = new_max - new_min;

        for (auto it = first; it != last; ++it)
            *it = new_min + (*it - cur_min) * new_range / cur_range;
    }

    template <typename It, typename T>
    void clamp_range(It first, It last, T newMin, T new_max)
    {
        for (auto it = first; it != last; ++it)
            Xyz::clamp(*it, newMin, new_max);
    }
}
