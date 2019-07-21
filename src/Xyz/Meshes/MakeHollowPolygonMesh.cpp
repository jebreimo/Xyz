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
    TriangleMesh<float>
    makeHollowPolygonMesh(short numberOfCorners, float innerRadius,
                          float outerRadius, float zValue)
    {
        if (numberOfCorners <= 2)
            return {};
        std::vector<Vector3f> points;
        points.reserve(size_t(numberOfCorners) * 2);
        std::vector<TriangleFace> indices;
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
            auto n = short(2 * i);
            indices.emplace_back(n, n + 3, n + 1);
            indices.emplace_back(n, n + 2, n + 3);
        }
        auto n = indices.size();
        indices[n - 1][2] = 1;
        indices[n - 1][1] = 0;
        indices[n - 2][1] = 1;
        return {move(points), move(indices)};
    }
}
