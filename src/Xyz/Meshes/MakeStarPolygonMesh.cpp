//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 23.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Meshes/MakeStarPolygonMesh.hpp"

namespace Xyz
{
    TriangleMesh<float>
    makeStarPolygonMesh(short numberOfCorners, float innerRadius,
                        float outerRadius, float zValue)
    {
        if (numberOfCorners <= 2)
            return {};
        std::vector<Vector3f> points;
        points.reserve(size_t(numberOfCorners) * 2);
        std::vector<TriangleFace> faces;
        faces.reserve(size_t(numberOfCorners) * 3);
        float halfAngle = PI_32 / numberOfCorners;
        for (short i = 0; i < numberOfCorners; ++i)
        {
            float angle = i * 2 * PI_32 / numberOfCorners;
            points.emplace_back(
                    makeVector(innerRadius * std::cos(angle - halfAngle),
                               innerRadius * std::sin(angle - halfAngle),
                               zValue));
            points.emplace_back(makeVector(outerRadius * std::cos(angle),
                                           outerRadius * std::sin(angle),
                                           zValue));
            auto index = short(2 * i);
            faces.emplace_back(index, index + 1, index + 2);
        }
        faces.back()[2] = 0;
        return {move(points), move(faces)};
    }
}
