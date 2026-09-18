//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.01.2012.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <type_traits>

#include "Orientation.hpp"
#include "Placement.hpp"

namespace Xyz
{
    template <std::floating_point T, unsigned N>
    class OrientedRectangle
    {
    public:
        /**
         * @brief The origin of the rectangle and the orientation of the
         *  length vector (pitch and yaw), and the width vector (roll).
         */
        Placement<T, N> placement = {};

        Vector<T, 2> size = {};
    };

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator==(const OrientedRectangle<T, N>& a, const OrientedRectangle<T, N>& b)
    {
        return a.placement == b.placement
            && a.size == b.size;
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator!=(const OrientedRectangle<T, N>& a, const OrientedRectangle<T, N>& b)
    {
        return !(a == b);
    }

    template <std::floating_point T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const OrientedRectangle<T, N>& rect)
    {
        return os << '{' << rect.placement << ", " << rect.size << "}";
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]] bool is_empty(const OrientedRectangle<T, N>& rect)
    {
        return rect.size.x() == 0 || rect.size.y() == 0;
    }

    template <std::floating_point T>
    [[nodiscard]]
    std::tuple<Vector<T, 2>, Vector<T, 2>>
    get_vectors(const OrientedRectangle<T, 2>& rect)
    {
        const auto [x, y] = get_vectors(rect.placement.orientation);
        return {x * rect.size.x(), y * rect.size.y()};
    }

    template <std::floating_point T>
    [[nodiscard]]
    std::tuple<Vector<T, 3>, Vector<T, 3>>
    get_vectors(const OrientedRectangle<T, 3>& rect)
    {
        const auto [x, y, _] = get_vectors(rect.placement.orientation);
        return {x * rect.size.x(), y * rect.size.y()};
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    Vector<T, 2> get_center(const OrientedRectangle<T, N>& rect)
    {
        const auto [x, y] = get_vectors(rect);
        return rect.placement.origin + (x + y) / T(2);
    }

    template <std::floating_point T, unsigned N>
    void set_center(OrientedRectangle<T, N>& rect,
                    const Vector<std::type_identity_t<T>, 2>& center)
    {
        const auto [x, y] = get_vectors(rect);
        rect.placement.origin = center - (x + y) / T(2);
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    OrientedRectangle<T, N> normalize(OrientedRectangle<T, N> rect)
    {
        const auto [x, y] = get_vectors(rect);
        if (rect.size.x() < 0)
        {
            rect.placement.origin += x;
            rect.size.x() = -rect.size.x();
        }

        if (rect.size.y() < 0)
        {
            rect.placement.origin += y;
            rect.size.y() = -rect.size.y();
        }

        rect.placement.orientation = normalize(rect.placement.orientation);

        if constexpr (N == 2)
        {
            constexpr auto pi = Constants<T>::PI;

            if (rect.placement.orientation.angle <= T(-0.5) * pi
                || T(0.5) * pi <= rect.placement.orientation.angle)
            {
                rect.placement.origin += x + y;
                if (rect.placement.orientation.angle < 0)
                    rect.placement.orientation.angle += pi;
                else
                    rect.placement.orientation.angle -= pi;
            }
        }

        return rect;
    }

    using OrientedRectangle2F = OrientedRectangle<float, 2>;
    using OrientedRectangle2D = OrientedRectangle<double, 2>;
    using OrientedRectangle3F = OrientedRectangle<float, 3>;
    using OrientedRectangle3D = OrientedRectangle<double, 3>;
}
