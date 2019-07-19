//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 26.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "Xyz/Vector.hpp"

namespace Xyz
{
    std::pair<std::vector<Vector3f>, std::vector<short>>
    makeHollowPolygon(short numberOfCorners, float innerRadius,
                      float outerRadius, float zValue);
}
