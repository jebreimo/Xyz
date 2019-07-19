//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-19.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Mesh
    {
    public:
        using Point = Vector<T, 3>;

        Mesh() = default;

        Mesh(std::vector<Point> points, std::vector<short> faces)
            : m_Points(move(points)),
              m_Faces(move(faces))
        {}

        const std::vector<Point>& points() const
        {
            return m_Points;
        }

        const std::vector<short>& faces() const
        {
            return m_Faces;
        }

        size_t sizeOfPoints() const
        {
            return m_Points.size() * sizeof(Point);
        }

        size_t sizeOfFaces() const
        {
            m_Faces.size() * sizof(ushort);
        }
    private:
        std::vector<Vector<T, 3>> m_Points;
        std::vector<short> m_Faces;
        std::vector<Vector<T, 2>> m_TextureCoordinates;
        std::vector<Vector<T, 3>> m_Normals;
    };
}
