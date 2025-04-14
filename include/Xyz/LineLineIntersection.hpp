//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 09.02.2016
//
// This file is distributed under the Zero-Clause BSD License.
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
    get_intersection_positions(const Line<T, 2>& a,
                               const Line<T, 2>& b,
                               Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto v_a = get_vector(a);
        auto n_b = get_normal(get_vector(b));

        Float denominator = dot(v_a, n_b);
        if (Approx<Float>(denominator, margin) == 0.0)
        {
            auto distance = dot(n_b, (get_point(a) - get_point(b)));
            if (Approx<Float>(distance, margin) == 0)
                return {LineRelationship::OVERLAPPING, Float(), Float()};
            else
                return {LineRelationship::NONINTERSECTING, Float(), Float()};
        }

        auto n_a = get_normal(get_vector(a));
        auto v_ab = get_point(b) - get_point(a);
        return {LineRelationship::INTERSECTING,
                dot(v_ab, n_b) / denominator,
                dot(v_ab, n_a) / denominator};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<LineRelationship, Float, Float>
    get_intersection_positions(const LineSegment<T, 2>& a,
                               const LineSegment<T, 2>& b,
                               Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto [rel, t0, t1] = get_intersection_positions(make_line(a),
                                                        make_line(b),
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
    get_projection_extent(const LineSegment<T, 2>& a,
                          const LineSegment<T, 2>& b,
                          Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto length = Float(get_length_squared(get_vector(a)));
        auto ta0 = dot(get_vector(a), (get_start(b) - get_start(a))) / length;
        auto ta1 = dot(get_vector(a), (get_end(b) - get_start(a))) / length;
        if ((ta0 > 1 && ta1 > 1) || (ta0 < 0 && ta1 < 0))
            return {false, {ta0, ta1}};
        ta0 = clamp<Float>(ta0, 0.0, 1.0);
        ta1 = clamp<Float>(ta1, 0.0, 1.0);
        return {true, {ta0, ta1}};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<bool, std::pair<Float, Float>, std::pair<Float, Float>>
    get_projection_extents(const LineSegment<T, 2>& a,
                           const LineSegment<T, 2>& b,
                           Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        auto [overlaps_a, offsets_a] = get_projection_extent(a, b, margin);
        auto [overlaps_b, offsets_b] = get_projection_extent(b, a, margin);
        if (offsets_a.first > offsets_a.second)
            std::swap(offsets_b.first, offsets_b.second);
        return {overlaps_a && overlaps_b, offsets_a, offsets_b};
    }

    template <typename T, typename Float = typename FloatType<T>::type>
    std::tuple<LineRelationship,
               std::pair<Float, Float>,
               std::pair<Float, Float>>
    get_intersection_extents(const LineSegment<T, 2>& a,
                             const LineSegment<T, 2>& b,
                             Float margin = Constants<Float>::DEFAULT_MARGIN)
    {
        using std::get;
        auto isect = get_intersection_positions(a, b, margin);
        if (get<0>(isect) == LineRelationship::COLINEAR)
        {
            auto overlap = get_projection_extents(a, b, margin);
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
