//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-05-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <array>
#include <cstdint>

namespace Xyz
{
    class SimplexNoise
    {
    public:
        SimplexNoise();

        double simplex(double x, double y, double z);

        double simplex(double x, double y, double z,
                       int octaves, double persistence);
    private:
        std::array<uint8_t, 512> permutation_;
    };
}
