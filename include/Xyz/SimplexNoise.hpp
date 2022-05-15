//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-05-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>

class SimplexNoise
{
public:
    SimplexNoise();

    double simplex(double x, double y, double z);

    double simplex(double x, double y, double z,
                   int octaves, double persistence);
private:
    uint8_t permutation_[512];
};
