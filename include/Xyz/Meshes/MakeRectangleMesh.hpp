//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-19.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include <Xyz/Vector.hpp>

namespace Xyz
{
    std::pair<std::vector<Vector3f>, std::vector<short>>
    makeRectangleMesh(float width = 2, float height = 2, float zValue = 0);
}
