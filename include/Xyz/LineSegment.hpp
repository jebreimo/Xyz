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
        LineSegment() = default;

        LineSegment(const Vector<T, N>& start, const Vector<T, N>& end)
            : m_start(start),
              m_end(end)
        {}

        const Vector<T, N>& end() const
        {
            return m_end;
        }

        void set_end(const Vector<T, N>& end)
        {
            m_end = end;
        }

        const Vector<T, N>& start() const
        {
            return m_start;
        }

        void set_start(const Vector<T, N>& start)
        {
            m_start = start;
        }
    private:
        Vector<T, N> m_start;
        Vector<T, N> m_end;
    };

    template <typename T, unsigned N>
    const Vector<T, N>& get_start(const LineSegment<T, N>& line)
    {
        return line.start();
    }

    template <typename T, unsigned N>
    const Vector<T, N>& get_end(const LineSegment<T, N>& line)
    {
        return line.end();
    }

    template <typename T, unsigned N>
    Vector<T, N> get_vector(const LineSegment<T, N>& line)
    {
        return get_end(line) - get_start(line);
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const LineSegment<T, N>& line)
    {
        return os << "{\"start\": " << get_start(line)
                  << ", \"end\": " << get_end(line) << "}";
    }

    template <typename T, unsigned N>
    Vector<T, N> get_point_at_t(const LineSegment<T, N>& line,
                                typename FloatType<T>::type t)
    {
        return get_start(line) + vector_cast<T>(get_vector(line) * t);
    }

    template <typename T, unsigned N>
    T get_coordinate_at_t(const LineSegment<T, N>& line, size_t coord,
                          typename FloatType<T>::type t)
    {
        auto v0 = get_start(line)[coord];
        auto v1 = get_end(line)[coord];
        return v0 + static_cast<T>(t * (v1 - v0));
    }

    template <typename T, unsigned N>
    LineSegment<T, N>
    make_line_segment(const Vector<T, N>& start, const Vector<T, N>& end)
    {
        return {start, end};
    }

    template <typename T, unsigned N>
    LineSegment<T, N>
    make_line_segment(const LineSegment<T, N>& base, T t0, T t1)
    {
        return {get_point_at_t(base, t0), get_point_at_t(base, t1)};
    }

    template <typename T, unsigned N>
    double get_length(const LineSegment<T, N>& line)
    {
        return get_length(get_vector(line));
    }

    template <typename T, unsigned N>
    LineSegment<T, N> get_reverse(const LineSegment<T, N>& line)
    {
        return {get_end(line), get_start(line)};
    }

    template <typename T, unsigned N>
    Vector<T, N> get_nearest_point(const LineSegment<T, N>& line,
                                   const Vector<T, N>& point)
    {
        auto divisor = get_length_squared(get_vector(line));
        auto t = (point - get_start(line)) * get_vector(line) / divisor;
        return get_point_at_t(line, clamp(t, 0.0, 1.0));
    }

    template <typename T>
    Vector<T, 2> get_relative_position(const LineSegment<T, 2>& line,
                                       const Vector<T, 2>& point)
    {
        auto lv = get_vector(line);
        auto len = get_length_squared(lv);
        auto pv = point - get_start(line);
        return make_vector2<T>(lv * pv / len, get_normal(lv) * pv / len);
    }

}
