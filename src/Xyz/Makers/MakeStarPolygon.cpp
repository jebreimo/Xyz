//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 23.04.2017.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Makers/MakeStarPolygon.hpp"

namespace Xyz
{
    std::pair<std::vector<Vector3f>, std::vector<short>>
    makeStarPolygon(short numberOfCorners, float innerRadius,
                    float outerRadius, float zValue)
    {
        if (numberOfCorners <= 2)
            return std::pair<std::vector<Vector3f>, std::vector<short>>();
        std::vector<Vector3f> points;
        points.reserve(size_t(numberOfCorners) * 2);
        std::vector<short> indices;
        indices.reserve(size_t(numberOfCorners) * 3);
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
            indices.push_back(short(2 * i));
            indices.push_back(short(2 * i + 1));
            indices.push_back(short(2 * i + 2));
        }
        indices.back() = 0;
        return std::make_pair(move(points), move(indices));
    }
}
