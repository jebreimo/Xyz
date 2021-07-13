//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-02-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "LineSegment.hpp"
#include "Vector.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class Line
    {
    public:
        Line() = default;

        Line(const Vector<T, N>& point, const Vector<T, N>& vector)
            : m_Point(point),
              m_Vector(vector)
        {}

        const Vector<T, N>& point() const
        {
            return m_Point;
        }

        void setPoint(const Vector<T, N>& point)
        {
            m_Point = point;
        }

        const Vector<T, N>& vector() const
        {
            return m_Vector;
        }

        void setVector(const Vector<T, N>& vector)
        {
            m_Vector = vector;
        }

    private:
        Vector<T, N> m_Point;
        Vector<T, N> m_Vector;
    };

    template <typename T, unsigned N>
    const Vector<T, N>& getPoint(const Line<T, N>& line)
    {
        return line.point();
    }

    template <typename T, unsigned N>
    const Vector<T, N>& getVector(const Line<T, N>& line)
    {
        return line.vector();
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Line<T, N>& line)
    {
        return os << "{\"vertex\": " << getPoint(line)
                  << ", \"vector\": " << getVector(line) << "}";
    }

    template <typename T, unsigned N>
    Line<T, N> makeLine(const Vector<T, N>& point,
                        const Vector<T, N>& vector)
    {
        return Line<T, N>(point, vector);
    }

    template <typename T, unsigned N>
    Line<T, N> makeLine(const LineSegment<T, N>& lineSegment)
    {
        return makeLine(getStart(lineSegment), getVector(lineSegment));
    }
}
