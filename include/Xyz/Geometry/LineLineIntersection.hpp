//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 09.02.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <tuple>
#include "Xyz/Utilities/Comparisons.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"

namespace Xyz
{
    enum class LineRelationship
    {
        /* Lines or line segments are parallel, but not co-linear.
         */
        NONINTERSECTING,
        /* Lines or line segments are intersecting.
         */
        INTERSECTING,
        /* Lines are overlapping.
         */
        OVERLAPPING,
        /* Line segments are co-linear and may or may not overlap.
         */
        COLINEAR
    };

    #define XYZ_CASE_OSTREAM_ENUM(name) \
        case name: return os << #name

    inline std::ostream& operator<<(std::ostream& os, LineRelationship e)
    {
        switch (e)
        {
        XYZ_CASE_OSTREAM_ENUM(LineRelationship::NONINTERSECTING);
        XYZ_CASE_OSTREAM_ENUM(LineRelationship::INTERSECTING);
        XYZ_CASE_OSTREAM_ENUM(LineRelationship::OVERLAPPING);
        XYZ_CASE_OSTREAM_ENUM(LineRelationship::COLINEAR);
        default: return os << "Unknown value.";
        }
    }

    template <typename T>
    std::tuple<LineRelationship, double, double>
    getIntersectionPositions(const Line<T, 2>& a,
                             const Line<T, 2>& b,
                             double epsilon)
    {
        typedef std::tuple<LineRelationship, double, double> Result;
        auto vA = getVector(a);
        auto nB = getNormal(getVector(b));

        auto denominator = static_cast<double>(dot(vA, nB));
        if (equal<double>(denominator, 0, epsilon))
        {
            if (equal<double>(dot(nB, (getPoint(a) - getPoint(b))), epsilon))
                return Result(LineRelationship::OVERLAPPING, 0.0, 0.0);
            else
                return Result(LineRelationship::NONINTERSECTING, 0.0, 0.0);
        }

        auto nA = getNormal(getVector(a));
        auto vAB = getPoint(b) - getPoint(a);
        return Result(LineRelationship::INTERSECTING,
                      dot(vAB, nB) / denominator,
                      dot(vAB, nA) / denominator);
    }

    template <typename T>
    std::tuple<LineRelationship, double, double>
    getIntersectionPositions(const LineSegment<T, 2>& a,
                             const LineSegment<T, 2>& b,
                             double epsilon)
    {
        auto ip = getIntersectionPositions(makeLine(a), makeLine(b), epsilon);
        if (std::get<0>(ip) == LineRelationship::OVERLAPPING)
        {
            std::get<0>(ip) = LineRelationship::COLINEAR;
            return ip;
        }
        else if (std::get<0>(ip) == LineRelationship::INTERSECTING
                 && between<double>(0, std::get<1>(ip), 1, epsilon)
                 && between<double>(0, std::get<2>(ip), 1, epsilon))
        {
            return ip;
        }
        else
        {
            std::get<0>(ip) = LineRelationship::NONINTERSECTING;
            return ip;
        }
    }

    template <typename T>
    std::pair<bool, std::pair<double, double>>
    getProjectionExtent(const LineSegment<T, 2>& a,
                        const LineSegment<T, 2>& b,
                        double epsilon)
    {
        auto length = getLengthSquared(getVector(a));
        auto ta0 = dot(getVector(a), (getStart(b) - getStart(a))) / length;
        auto ta1 = dot(getVector(a), (getEnd(b) - getStart(a))) / length;
        if ((ta0 > 1 && ta1 > 1) || (ta0 < 0 && ta1 < 0))
            return std::make_pair(false, std::make_pair(ta0, ta1));
        ta0 = getClamped(ta0, 0.0, 1.0);
        ta1 = getClamped(ta1, 0.0, 1.0);
        return std::make_pair(true, std::make_pair(ta0, ta1));
    }

    template <typename T>
    std::tuple<bool, std::pair<double, double>, std::pair<double, double>>
    getProjectionExtents(const LineSegment<T, 2>& a,
                         const LineSegment<T, 2>& b,
                         double epsilon)
    {
        bool overlapsA;
        std::pair<double, double> offsetsA;
        std::tie(overlapsA, offsetsA) = getProjectionExtent(a, b, epsilon);
        bool overlapsB;
        std::pair<double, double> offsetsB;
        std::tie(overlapsB, offsetsB) = getProjectionExtent(b, a, epsilon);
        if (offsetsA.first > offsetsA.second)
            std::swap(offsetsB.first, offsetsB.second);
        return std::make_tuple(overlapsA && overlapsB, offsetsA, offsetsB);
    }

    template <typename T>
    std::tuple<LineRelationship,
               std::pair<double, double>,
               std::pair<double, double>>
    getIntersectionExtents(const LineSegment<T, 2>& a,
                           const LineSegment<T, 2>& b,
                           double epsilon)
    {
        using namespace std;
        auto isect = getIntersectionPositions(a, b, epsilon);
        if (get<0>(isect) == LineRelationship::COLINEAR)
        {
            auto overlap = getProjectionExtents(a, b, epsilon);
            return make_tuple(get<0>(overlap)
                              ? LineRelationship::INTERSECTING
                              : LineRelationship::NONINTERSECTING,
                              get<1>(overlap), get<2>(overlap));
        }
        else
        {
            return make_tuple(get<0>(isect),
                              make_pair(get<1>(isect), get<1>(isect)),
                              make_pair(get<2>(isect), get<2>(isect)));
        }
    }
}
