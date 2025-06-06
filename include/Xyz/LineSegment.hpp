//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-02-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Clamp.hpp"
#include "FloatType.hpp"
#include "Vector.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class LineSegment
    {
    public:
        using FloatT = typename FloatType<T>::type;

        Vector<T, N> start;
        Vector<T, N> end;

        constexpr LineSegment() = default;

        constexpr LineSegment(const Vector<T, N>& start, const Vector<T, N>& end)
            : start(start),
              end(end)
        {}

        Vector<T, N> vector() const
        {
            return end - start;
        }

        LineSegment segment(FloatT t0, FloatT t1) const
        {
            auto vec = vector_cast<T>(vector());
            return {
                start + vector_cast<T>(vec * t0),
                start + vector_cast<T>(vec * t1)
            };
        }
    };

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const LineSegment<T, N>& line)
    {
        return os << "{\"start\": " << line.start
            << ", \"end\": " << line.end << "}";
    }

    template <typename T, unsigned N>
    Vector<T, N> get_point_at_t(const LineSegment<T, N>& line,
                                typename FloatType<T>::type t)
    {
        return line.start + vector_cast<T>(line.vector() * t);
    }

    // template <typename T, unsigned N>
    // T get_coordinate_at_t(const LineSegment<T, N>& line, size_t coord,
    //                       typename FloatType<T>::type t)
    // {
    //     auto v0 = line.start[coord];
    //     auto v1 = line.end[coord];
    //     return v0 + static_cast<T>(t * (v1 - v0));
    // }

    template <typename T, unsigned N>
    double get_length(const LineSegment<T, N>& line)
    {
        return get_length(line.vector());
    }

    template <typename T, unsigned N>
    LineSegment<T, N> get_reverse(const LineSegment<T, N>& line)
    {
        return {line.end, line.start};
    }

    template <typename T, unsigned N>
    Vector<T, N> get_nearest_point(const LineSegment<T, N>& line,
                                   const Vector<T, N>& point)
    {
        auto divisor = get_length_squared(line.vector());
        auto t = (point - line.start) * line.vector() / divisor;
        return get_point_at_t(line, clamp(t, 0.0, 1.0));
    }

    template <typename T>
    Vector<T, 2> get_relative_position(const LineSegment<T, 2>& line,
                                       const Vector<T, 2>& point)
    {
        auto lv = line.vector();
        auto len = get_length_squared(lv);
        auto pv = point - line.start;
        return make_vector2<T>(lv * pv / len, get_normal(lv) * pv / len);
    }
}
