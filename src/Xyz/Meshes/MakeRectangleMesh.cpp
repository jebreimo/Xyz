//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-19.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Meshes/MakeRectangleMesh.hpp"

namespace Xyz
{
    std::pair<std::vector<Vector3f>, std::vector<short>>
    makeRectangleMesh(float width, float height, float zValue)
    {
        float hw = width / 2;
        float hh = height / 2;
        std::vector<Vector3f> points {
            {-hw, -hh, zValue},
            {hw, -hh, zValue},
            {hw, hh, zValue},
            {-hw, hh, zValue}};
        std::vector<short> faces {
            0, 1, 2,
            0, 2, 3
        };
        return {move(points), move(faces)};
    }
}
