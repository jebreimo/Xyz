//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-02-07.
//
// This file is distributed under the Zero-Clause BSD License.
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
        Vector<T, N> point;
        Vector<T, N> vector;

        Line() = default;

        Line(const Vector<T, N>& point, const Vector<T, N>& vector)
            : point(point),
              vector(vector)
        {}
    };

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Line<T, N>& line)
    {
        return os << "{\"vertex\": " << get_point(line)
                  << ", \"vector\": " << get_vector(line) << "}";
    }

    template <typename T, unsigned N>
    Line<T, N> make_line(const Vector<T, N>& point,
                         const Vector<T, N>& vector)
    {
        return Line<T, N>(point, vector);
    }

    template <typename T, unsigned N>
    Line<T, N> make_line(const LineSegment<T, N>& line_segment)
    {
        return {line_segment.start, line_segment.end - line_segment.start};
    }
}
