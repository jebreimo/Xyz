//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 09.02.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <tuple>
#include "../Utilities/Comparisons.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"

namespace Xyz {

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

    #define CASE_OSTREAM_ENUM(name) \
        case name: return os << #name

    inline std::ostream& operator<<(std::ostream& os, LineRelationship e)
    {
        switch (e)
        {
        CASE_OSTREAM_ENUM(LineRelationship::NONINTERSECTING);
        CASE_OSTREAM_ENUM(LineRelationship::INTERSECTING);
        CASE_OSTREAM_ENUM(LineRelationship::OVERLAPPING);
        CASE_OSTREAM_ENUM(LineRelationship::COLINEAR);
        default: return os << "Unknown value.";
        }
    }

    template <typename T>
    std::tuple<LineRelationship, double, double> getIntersectionFactors(
             const Line<T, 2>& a,
             const Line<T, 2>& b,
             double epsilon)
    {
        typedef std::tuple<LineRelationship, double, double> Result;
        auto vA = getVector(a);
        auto nB = getNormal(getVector(b));

        auto denominator = static_cast<double>(vA * nB);
        if (equal<double>(denominator, 0, epsilon))
        {
            if (equal<double>(nB * (getPoint(a) - getPoint(b)), epsilon))
                return Result(LineRelationship::OVERLAPPING, 0.0, 0.0);
            else
                return Result(LineRelationship::NONINTERSECTING, 0.0, 0.0);
        }

        auto nA = getNormal(getVector(a));
        auto vAB = getPoint(b) - getPoint(a);
        return Result(LineRelationship::INTERSECTING,
                      vAB * nB / denominator,
                      vAB * nA / denominator);
    }

    template <typename T>
    std::tuple<LineRelationship, double, double> getIntersectionFactors(
            const LineSegment<T, 2>& a,
            const LineSegment<T, 2>& b,
            double epsilon)
    {
        auto res = getIntersectionFactors(makeLine(a), makeLine(b), epsilon);
        if (std::get<0>(res) == LineRelationship::OVERLAPPING)
        {
            std::get<0>(res) = LineRelationship::COLINEAR;
            return res;
        }
        else if (std::get<0>(res) == LineRelationship::INTERSECTING
                 && inRange<double>(0, std::get<1>(res), 1, epsilon)
                 && inRange<double>(0, std::get<2>(res), 1, epsilon))
        {
            return res;
        }
        else
        {
            std::get<0>(res) = LineRelationship::NONINTERSECTING;
            return res;
        }
    }

}
