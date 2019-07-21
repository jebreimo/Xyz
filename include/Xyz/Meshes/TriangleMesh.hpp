//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-19.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "Xyz/Vector.hpp"
#include "TriangleFace.hpp"

namespace Xyz
{
    template <typename T>
    class TriangleMesh
    {
    public:
        using Point2 = Vector<T, 2>;
        using Point3 = Vector<T, 3>;

        TriangleMesh() = default;

        TriangleMesh(std::vector<Point3> points,
                     std::vector<TriangleFace> faces,
                     std::vector<Point3> normals = {},
                     std::vector<Point2> texturePoints = {})
            : m_Points(move(points)),
              m_Faces(move(faces)),
              m_Normals(move(normals)),
              m_TexturePoints(move(texturePoints))
        {
            if (!m_Normals.empty() && m_Normals.size() != m_Points.size())
                XYZ_THROW("The size of normals and points are unequal.");
            if (!m_TexturePoints.empty() && m_TexturePoints.size() != m_Points.size())
                XYZ_THROW("The size of normals and points are unequal.");
        }

        const std::vector<Point3>& points() const
        {
            return m_Points;
        }

        const std::vector<Point3>& normals() const
        {
            return m_Normals;
        }

        const std::vector<Point2>& texturePoints() const
        {
            return m_TexturePoints;
        }

        const std::vector<TriangleFace>& faces() const
        {
            return m_Faces;
        }

        size_t hasNormals() const
        {
            return !m_Normals.empty();
        }

        size_t hasTexturePoints() const
        {
            return !m_TexturePoints.empty();
        }
    private:
        std::vector<Vector<T, 3>> m_Points;
        std::vector<TriangleFace> m_Faces;
        std::vector<Vector<T, 3>> m_Normals;
        std::vector<Vector<T, 2>> m_TexturePoints;
    };

    template <typename T, unsigned N>
    size_t copyPoints(const std::vector<Vector<T, N>>& points,
                      float* dest, size_t size, size_t rowSize, size_t offset)
    {
        if (size / rowSize < points.size())
            XYZ_THROW("Destination's size is too small.");
        if (rowSize < offset + N)
            XYZ_THROW("Row size is too small.");
        for (auto& point : points)
        {
            auto values = dest + offset;
            for (unsigned i = 0; i < N; ++i)
                values[i] = float(point[i]);
            dest += rowSize;
        }
        return offset + N;
    }
}
