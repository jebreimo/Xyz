//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

namespace Xyz
{
    struct TriangleFace
    {
        TriangleFace() = default;

        TriangleFace(short a, short b, short c)
        {
            indices[0] = a;
            indices[1] = b;
            indices[2] = c;
        }

        short& operator[](size_t i)
        {
            return indices[i];
        }
        
        short operator[](size_t i) const 
        {
            return indices[i];
        }
        
        std::array<short, 3> indices = {};
    };
}
