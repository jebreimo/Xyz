//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

namespace Xyz
{
    template <typename T>
    struct FloatType
    {
        using type = double;
    };

    template <>
    struct FloatType<float>
    {
        using type = float;
    };

    template <>
    struct FloatType<long double>
    {
        using type = long double;
    };
}
