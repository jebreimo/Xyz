//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <cmath>
#include "Constants.hpp"

namespace Xyz
{
    template <typename T>
    [[nodiscard]] T clamp(T value, T min, T max)
    {
        return std::min(max, std::max(min, value));
    }

    template <typename It, typename T>
    void scale_range(It first, It last, T new_min, T new_max)
    {
        if (first == last)
            return;

        auto its = std::minmax_element(first, last);
        auto [cur_min, cur_max] = *its;
        auto cur_range = cur_max - cur_min;

        if (cur_range == 0)
        {
            std::fill(first, last, new_min);
            return;
        }

        auto new_range = new_max - new_min;

        for (auto it = first; it != last; ++it)
            *it = new_min + (*it - cur_min) * new_range / cur_range;
    }

    template <typename It, typename T>
    void clamp_range(It first, It last, T newMin, T new_max)
    {
        for (auto it = first; it != last; ++it)
            *it = Xyz::clamp(*it, newMin, new_max);
    }
}
