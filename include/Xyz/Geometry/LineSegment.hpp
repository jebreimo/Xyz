//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-02-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Vector.hpp"
#include "Xyz/Utilities/Clamp.hpp"

namespace Xyz {

    template <typename T, unsigned N>
    class LineSegment
    {
    public:
        LineSegment()
        {}

        LineSegment(const Vector<T, N>& start, const Vector<T, N>& end)
            : m_Start(start),
              m_End(end)
        {}

        const Vector<T, N>& end() const
        {
            return m_End;
        }

        void setEnd(const Vector<T, N>& end)
        {
            m_End = end;
        }

        const Vector<T, N>& start() const
        {
            return m_Start;
        }

        void setStart(const Vector<T, N>& start)
        {
            m_Start = start;
        }
    private:
        Vector<T, N> m_Start;
        Vector<T, N> m_End;
    };

    template <typename T, unsigned N>
    const Vector<T, N>& getStart(const LineSegment<T, N>& line)
    {
        return line.start();
    }

    template <typename T, unsigned N>
    const Vector<T, N>& getEnd(const LineSegment<T, N>& line)
    {
        return line.end();
    }

    template <typename T, unsigned N>
    Vector<T, N> getVector(const LineSegment<T, N>& line)
    {
        return getEnd(line) - getStart(line);
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const LineSegment<T, N>& line)
    {
        return os << "{\"start\": " << getStart(line)
                  << ", \"end\": " << getEnd(line) << "}";
    }

    template <typename T, unsigned N>
    Vector<T, N> getPointAtT(const LineSegment<T, N>& line, double t)
    {
        return getStart(line) + getVector(line) * t;
    }

    template <typename T, unsigned N>
    T getCoordinateAtT(const LineSegment<T, N>& line, size_t coord, double t)
    {
        auto v0 = getStart(line)[coord];
        auto v1 = getEnd(line)[coord];
        return v0 + static_cast<T>(t * (v1 - v0));
    }

    template <typename T, unsigned N>
    LineSegment<T, N> makeLineSegment(const Vector<T, N>& start,
                                      const Vector<T, N>& end)
    {
        return LineSegment<T, N>(start, end);
    }

    template <typename T, unsigned N>
    double getLength(const LineSegment<T, N>& line)
    {
        return getLength(getVector(line));
    }

    template <typename T, unsigned N>
    LineSegment<T, N> getReverse(const LineSegment<T, N>& line)
    {
        return LineSegment<T, N>(getEnd(line), getStart(line));
    }

    template <typename T, unsigned N>
    Vector<T, N> getNearestPoint(const LineSegment<T, N>& line,
                                 const Vector<T, N>& point)
    {
        auto divisor = getLengthSquared(getVector(line));
        auto t = (point - getStart(line)) * getVector(line) / divisor;
        return getPointAtT(line, getClamped(t, 0.0, 1.0));
    }

    template <typename T>
    Vector<T, 2> getRelativePosition(const LineSegment<T, 2>& line,
                                     const Vector<T, 2>& point)
    {
        auto lv = getVector(line);
        auto len = getLengthSquared(lv);
        auto pv = point - getStart(line);
        return makeVector2<T>(lv * pv / len, getNormal(lv) * pv / len);
    }

}
