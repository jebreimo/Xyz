//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 09.02.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <tuple>
#include "Approx.hpp"
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

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<LineRelationship, Float, Float>
    getIntersectionPositions(const Line<T, 2>& a,
                             const Line<T, 2>& b,
                             Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto vA = getVector(a);
        auto nB = getNormal(getVector(b));

        Float denominator = dot(vA, nB);
        if (Approx<Float>(denominator, margin) == 0.0)
        {
            auto distance = dot(nB, (getPoint(a) - getPoint(b)));
            if (Approx<Float>(distance, margin) == 0)
                return {LineRelationship::OVERLAPPING, Float(), Float()};
            else
                return {LineRelationship::NONINTERSECTING, Float(), Float()};
        }

        auto nA = getNormal(getVector(a));
        auto vAB = getPoint(b) - getPoint(a);
        return {LineRelationship::INTERSECTING,
                dot(vAB, nB) / denominator,
                dot(vAB, nA) / denominator};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<LineRelationship, Float, Float>
    getIntersectionPositions(const LineSegment<T, 2>& a,
                             const LineSegment<T, 2>& b,
                             Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto [rel, t0, t1] = getIntersectionPositions(makeLine(a),
                                                      makeLine(b),
                                                      margin);
        if (rel == LineRelationship::OVERLAPPING)
        {
            return {LineRelationship::COLINEAR, t0, t1};
        }
        else if (rel == LineRelationship::INTERSECTING
                 && 0.0 < Approx<Float>(t0, margin)
                 && Approx<Float>(t0, margin) < 1.0
                 && 0.0 < Approx<Float>(t1, margin)
                 && Approx<Float>(t1, margin) < 1.0)
        {
            return {rel, t0, t1};
        }
        else
        {
            return {LineRelationship::NONINTERSECTING, t0, t1};
        }
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::pair<bool, std::pair<Float, Float>>
    getProjectionExtent(const LineSegment<T, 2>& a,
                        const LineSegment<T, 2>& b,
                        Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto length = Float(getLengthSquared(getVector(a)));
        auto ta0 = dot(getVector(a), (getStart(b) - getStart(a))) / length;
        auto ta1 = dot(getVector(a), (getEnd(b) - getStart(a))) / length;
        if ((ta0 > 1 && ta1 > 1) || (ta0 < 0 && ta1 < 0))
            return {false, {ta0, ta1}};
        ta0 = getClamped<Float>(ta0, 0.0, 1.0);
        ta1 = getClamped<Float>(ta1, 0.0, 1.0);
        return {true, {ta0, ta1}};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<bool, std::pair<Float, Float>, std::pair<Float, Float>>
    getProjectionExtents(const LineSegment<T, 2>& a,
                         const LineSegment<T, 2>& b,
                         Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto [overlapsA, offsetsA] = getProjectionExtent(a, b, margin);
        auto [overlapsB, offsetsB] = getProjectionExtent(b, a, margin);
        if (offsetsA.first > offsetsA.second)
            std::swap(offsetsB.first, offsetsB.second);
        return {overlapsA && overlapsB, offsetsA, offsetsB};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<LineRelationship,
               std::pair<Float, Float>,
               std::pair<Float, Float>>
    getIntersectionExtents(const LineSegment<T, 2>& a,
                           const LineSegment<T, 2>& b,
                           Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        using std::get;
        auto isect = getIntersectionPositions(a, b, margin);
        if (get<0>(isect) == LineRelationship::COLINEAR)
        {
            auto overlap = getProjectionExtents(a, b, margin);
            return {get<0>(overlap) ? LineRelationship::INTERSECTING
                                    : LineRelationship::NONINTERSECTING,
                    get<1>(overlap), get<2>(overlap)};
        }
        else
        {
            return {get<0>(isect),
                    {get<1>(isect), get<1>(isect)},
                    {get<2>(isect), get<2>(isect)}};
        }
    }
}
