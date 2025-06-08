//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.01.2012.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cmath>
#include <type_traits>
#include "Utilities.hpp"
#include "Vector.hpp"

namespace Xyz
{
    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    class Rectangle
    {
    public:
        using Vector = Vector<T, 2>;

        /**
         * @brief The origin of the rectangle.
         */
        Vector origin;

        /**
         * @brief The size of the rectangle.
         *
         * The first value is the length, and the second value is the width.
         */
        Vector size;

        /**
         * @brief The angle of the rectangle in radians.
         *
         * The angle is measured counter-clockwise from the positive x-axis
         * to the "length" of the rectangle, that is, the first value in size.
         */
        T angle = {};

        Rectangle() = default;

        Rectangle(const Vector& origin,
                   const Vector& size,
                   T angle = {})
            : origin(origin), size(size), angle(angle)
        {}

        [[nodiscard]] Vector length_vector() const
        {
            return size[0] * Vector(std::cos(angle), std::sin(angle));
        }

        [[nodiscard]] Vector width_vector() const
        {
            return size[1] * Vector(-std::sin(angle), std::cos(angle));
        }

        [[nodiscard]]
        Vector point(size_t index) const
        {
            switch (index % 4)
            {
            default:
            case 0:
                return origin;
            case 1:
                return origin + length_vector();
            case 2:
                return origin + length_vector() + width_vector();
            case 3:
                return origin + width_vector();
            }
        }
    };

    template <typename T>
    [[nodiscard]]
    bool operator==(const Rectangle<T>& a, const Rectangle<T>& b)
    {
        return a.origin == b.origin && a.size == b.size && a.angle == b.angle;
    }

    template <typename T>
    [[nodiscard]]
    bool operator!=(const Rectangle<T>& a, const Rectangle<T>& b)
    {
        return !(a == b);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Rectangle<T>& rect)
    {
        return os << '{' << rect.origin
            << ", " << rect.size
            << ", " << rect.angle << "}";
    }

    template <typename T>
    [[nodiscard]] bool is_empty(const Rectangle<T>& rect)
    {
        return rect.size[0] == 0 || rect.size[1] == 0;
    }

    /**
     * @brief Returns the bounding box of a rotated rectangle.
     *
     * The bounding box is a rectangle with no rotation, and origin in
     * the bottom left corner.
     */
    template <typename T>
    [[nodiscard]]
    typename Rectangle<T>::FloatVector bounding_box(const Rectangle<T>& rect)
    {
        typename Rectangle<T>::FloatVector p0, p1;
        p0 = p1 = rect.origin;
        for (size_t i = 1; i < 4; ++i)
        {
            auto p = rect.point(i);
            p0[0] = std::min(p0[0], p[0]);
            p0[1] = std::min(p0[1], p[1]);
            p1[0] = std::max(p1[0], p[0]);
            p1[1] = std::max(p1[1], p[1]);
        }

        return {p0, p1 - p0};
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_center(const Rectangle<T>& rect)
    {
        return rect.origin + (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <typename T>
    void set_center(Rectangle<T>& rect,
                    const Vector<std::type_identity_t<T>, 2>& center)
    {
        rect.origin = center - (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <typename T>
    [[nodiscard]]
    Rectangle<T> normalize(Rectangle<T> rect)
    {
        if (rect.size[0] < 0)
        {
            rect.origin += rect.length_vector();
            rect.size[0] = -rect.size[0];
        }

        if (rect.size[1] < 0)
        {
            rect.origin += rect.width_vector();
            rect.size[1] = -rect.size[1];
        }

        constexpr auto pi = Constants<T>::PI;

        rect.angle = to_principal_angle(rect.angle);
        if (T(0.5) * pi < rect.angle && rect.angle < T(1.5) * pi)
        {
            rect.origin += rect.length_vector() + rect.width_vector();
            if (rect.angle < pi)
                rect.angle += pi;
            else
                rect.angle -= pi;
        }

        return rect;
    }

    using RectangleF = Rectangle<float>;
    using RectangleD = Rectangle<double>;
}
