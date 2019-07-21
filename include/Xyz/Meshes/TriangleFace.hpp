//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>

namespace Xyz
{
    struct TriangleFace
    {
        TriangleFace() = default;

        TriangleFace(uint16_t a, uint16_t b, uint16_t c)
        {
            indices[0] = a;
            indices[1] = b;
            indices[2] = c;
        }

        uint16_t& operator[](size_t i)
        {
            return indices[i];
        }

        uint16_t operator[](size_t i) const
        {
            return indices[i];
        }

        std::array<uint16_t, 3> indices = {};
    };
}
