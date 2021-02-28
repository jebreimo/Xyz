//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-17.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include <cstdlib>
#include "Xyz/Constants.hpp"

namespace Xyz
{
    template <typename T>
    bool equal(T a, T b, T margin)
    {
        return std::abs(a - b) <= margin;
    }

    template <typename T>
    bool equal(T a, T b)
    {
        if constexpr (std::is_floating_point_v<T>)
            return equal(a, b, Constants<T>::DEFAULT_MARGIN);
        else
            return a == b;
    }

    template <typename T>
    bool less(T a, T b, T margin)
    {
        return a < b - margin;
    }

    template <typename T>
    bool lessOrEqual(T a, T b, T margin)
    {
        return a <= b + margin;
    }

    template <typename T>
    bool greater(T a, T b, T margin)
    {
        return a > b + margin;
    }

    template <typename T>
    bool greaterOrEqual(T a, T b, T margin)
    {
        return a >= b - margin;
    }

    template <typename T>
    bool between(T from, T value, T to, T margin)
    {
        return greaterOrEqual(value, from, margin)
               && lessOrEqual(value, to, margin);
    }

    template <typename T>
    bool betweenExEx(T from, T value, T to, T margin)
    {
        return greater(value, from, margin) && less(value, to, margin);
    }

    template <typename T>
    bool betweenIncEx(T from, T value, T to, T margin)
    {
        return greaterOrEqual(value, from, margin)
               && less(value, to, margin);
    }

    template <typename T>
    bool betweenExInc(T from, T value, T to, T margin)
    {
        return greater(value, from, margin)
               && lessOrEqual(value, to, margin);
    }
}
