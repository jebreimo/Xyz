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
#include "FloatType.hpp"
#include "IntersectionType.hpp"
#include "Line.hpp"
#include "LineSegment.hpp"

namespace Xyz
{
    template <typename T, typename Float = FloatType_t<T>>
    std::tuple<IntersectionType, Float, Float>
    get_intersection_positions(const Line<T, 2>& a,
                               const Line<T, 2>& b,
                               Float margin = Margin<Float>::DEFAULT)
    {
        auto v_a = get_vector(a);
        auto n_b = get_normal(get_vector(b));

        Float denominator = dot(v_a, n_b);
        if (Approx<Float>(denominator, margin) == 0.0)
        {
            auto distance = dot(n_b, get_point(a) - get_point(b));
            if (Approx<Float>(distance, margin) == 0)
                return {IntersectionType::OVERLAPPING, Float(), Float()};

            return {IntersectionType::NON_INTERSECTING, Float(), Float()};
        }

        auto n_a = get_normal(get_vector(a));
        auto v_ab = get_point(b) - get_point(a);
        return {
            IntersectionType::INTERSECTING,
            dot(v_ab, n_b) / denominator,
            dot(v_ab, n_a) / denominator
        };
    }

    template <typename T, typename Float = FloatType_t<T>>
    std::tuple<IntersectionType, Float, Float>
    get_intersection_positions(const LineSegment<T, 2>& a,
                               const LineSegment<T, 2>& b,
                               Float margin = Margin<Float>::DEFAULT)
    {
        auto [rel, t0, t1] = get_intersection_positions(make_line(a),
                                                        make_line(b),
                                                        margin);
        if (rel == IntersectionType::OVERLAPPING)
        {
            return {IntersectionType::COLINEAR, t0, t1};
        }

        if (rel == IntersectionType::INTERSECTING
            && 0.0 < Approx<Float>(t0, margin)
            && Approx<Float>(t0, margin) < 1.0
            && 0.0 < Approx<Float>(t1, margin)
            && Approx<Float>(t1, margin) < 1.0)
        {
            return {rel, t0, t1};
        }

        return {IntersectionType::NON_INTERSECTING, t0, t1};
    }

    template <typename T, typename Float = FloatType_t<T>>
    std::pair<bool, std::pair<Float, Float>>
    get_projection_extent(const LineSegment<T, 2>& a,
                          const LineSegment<T, 2>& b,
                          Float margin = Margin<Float>::DEFAULT)
    {
        auto length = Float(get_length_squared(a.vector()));
        auto ta0 = dot(a.vector(), b.start - a.start) / length;
        auto ta1 = dot(a.vector(), b.end - a.start) / length;
        if ((Approx<Float>(ta0, margin) > 1
                && Approx<Float>(ta1, margin) > 1)
            || (Approx<Float>(ta0, margin) < 0
                && Approx<Float>(ta1, margin) < 0))
        {
            return {false, {ta0, ta1}};
        }
        ta0 = clamp<Float>(ta0, 0.0, 1.0);
        ta1 = clamp<Float>(ta1, 0.0, 1.0);
        return {true, {ta0, ta1}};
    }

    template <typename T, typename Float = FloatType_t<T>>
    std::tuple<bool, std::pair<Float, Float>, std::pair<Float, Float>>
    get_projection_extents(const LineSegment<T, 2>& a,
                           const LineSegment<T, 2>& b,
                           Float margin = Margin<Float>::DEFAULT)
    {
        auto [overlaps_a, offsets_a] = get_projection_extent(a, b, margin);
        auto [overlaps_b, offsets_b] = get_projection_extent(b, a, margin);
        if (offsets_a.first > offsets_a.second)
            std::swap(offsets_b.first, offsets_b.second);
        return {overlaps_a && overlaps_b, offsets_a, offsets_b};
    }

    template <typename T, typename Float = FloatType_t<T>>
    std::tuple<IntersectionType,
               std::pair<Float, Float>,
               std::pair<Float, Float>>
    get_intersection_extents(const LineSegment<T, 2>& a,
                             const LineSegment<T, 2>& b,
                             Float margin = Margin<Float>::basic())
    {
        using std::get;
        auto isect = get_intersection_positions(a, b, margin);
        if (get<0>(isect) == IntersectionType::COLINEAR)
        {
            auto overlap = get_projection_extents(a, b, margin);
            return {
                get<0>(overlap)
                    ? IntersectionType::INTERSECTING
                    : IntersectionType::NON_INTERSECTING,
                get<1>(overlap), get<2>(overlap)
            };
        }

        return {
            get<0>(isect),
            {get<1>(isect), get<1>(isect)},
            {get<2>(isect), get<2>(isect)}
        };
    }
}
