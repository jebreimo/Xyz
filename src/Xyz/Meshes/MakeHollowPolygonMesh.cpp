//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 26.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Meshes/MakeHollowPolygonMesh.hpp"

#include "Xyz/Utilities/Constants.hpp"

namespace Xyz
{
    std::pair<std::vector<Vector3f>, std::vector<short>>
    makeHollowPolygonMesh(short numberOfCorners, float innerRadius,
                          float outerRadius, float zValue)
    {
        if (numberOfCorners <= 2)
            return std::pair<std::vector<Vector3f>, std::vector<short>>();
        std::vector<Vector3f> points;
        points.reserve(size_t(numberOfCorners) * 2);
        std::vector<short> indices;
        indices.reserve(size_t(numberOfCorners) * 3);
        for (short i = 0; i < numberOfCorners; ++i)
        {
            float angle = i * 2 * PI_32 / numberOfCorners;
            auto xFactor = std::cos(angle);
            auto yFactor = std::sin(angle);
            points.emplace_back(makeVector(innerRadius * xFactor,
                                           innerRadius * yFactor,
                                           zValue));
            points.emplace_back(makeVector(outerRadius * xFactor,
                                           outerRadius * yFactor,
                                           zValue));
            indices.push_back(short(2 * i));
            indices.push_back(short(2 * i + 3));
            indices.push_back(short(2 * i + 1));
            indices.push_back(short(2 * i));
            indices.push_back(short(2 * i + 2));
            indices.push_back(short(2 * i + 3));
        }
        auto n = indices.size();
        indices[n - 1] = 1;
        indices[n - 2] = 0;
        indices[n - 5] = 1;
        return std::make_pair(move(points), move(indices));
    }
}
