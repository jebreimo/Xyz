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

#include "FloatType.hpp"
#include "Orientation.hpp"
#include "Utilities.hpp"
#include "Vector.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class Rectangle
    {
    public:
        using VectorT = Vector<T, N>;

        /**
         * @brief The origin of the rectangle.
         */
        VectorT origin;

        /**
         * @brief The size of the rectangle.
         *
         * The first value is the length, and the second value is the width.
         */
        union
        {
            T size[2] = {0, 0};

            struct
            {
                T length; ///< The length of the rectangle.
                T width; ///< The width of the rectangle.
            };
        };

        /**
         * @brief The angle of the rectangle in radians.
         *
         * The angle is measured counter-clockwise from the positive x-axis
         * to the "length" of the rectangle, that is, the first value in size.
         */
        Orientation<T, N> orientation = {};

        Rectangle() = default;

        Rectangle(const VectorT& origin,
                  T length, T width,
                  Orientation<T, N> orientation = {})
            : origin(origin),
              length(length),
              width(width),
              orientation(orientation)
        {}

        [[nodiscard]] VectorT length_vector() const
        {
            return length * get_x_vector(orientation);
        }

        [[nodiscard]] VectorT width_vector() const
        {
            return width * get_y_vector(orientation);
        }

        [[nodiscard]]
        constexpr VectorT operator[](size_t index) const
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

    template <typename T, unsigned N>
    [[nodiscard]]
    bool operator==(const Rectangle<T, N>& a, const Rectangle<T, N>& b)
    {
        return a.origin == b.origin
            && a.length == b.length
            && a.width == b.width
            && a.orientation == b.orientation;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    bool operator!=(const Rectangle<T, N>& a, const Rectangle<T, N>& b)
    {
        return !(a == b);
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Rectangle<T, N>& rect)
    {
        return os << '{' << rect.origin
            << ", " << Vector(rect.size)
            << ", " << rect.orientation << "}";
    }

    template <typename T, unsigned N>
    [[nodiscard]] bool is_empty(const Rectangle<T, N>& rect)
    {
        return rect.length == 0 || rect.width == 0;
    }

    /**
     * @brief Returns the bounding box of a rotated rectangle.
     *
     * The bounding box is a rectangle with no rotation, and origin in
     * the bottom left corner.
     */
    template <typename T, unsigned N>
    [[nodiscard]]
    typename Rectangle<T, N>::Vector bounding_box(const Rectangle<T, N>& rect)
    {
        typename Rectangle<T, N>::Vector p0, p1;
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

    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, 2> get_center(const Rectangle<T, N>& rect)
    {
        return rect.origin + (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <typename T, unsigned N>
    void set_center(Rectangle<T, N>& rect,
                    const Vector<std::type_identity_t<T>, 2>& center)
    {
        rect.origin = center - (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    Rectangle<T, N> normalize(Rectangle<T, N> rect)
    {
        if (rect.length < 0)
        {
            rect.origin += rect.length_vector();
            rect.length = -rect.length;
        }

        if (rect.width < 0)
        {
            rect.origin += rect.width_vector();
            rect.width = -rect.width;
        }

        rect.orientation = normalize(rect.orientation);

        if constexpr (N == 2)
        {
            constexpr auto pi = Constants<T>::PI;
            using FloatT = FloatType_t<T>;

            if (rect.orientation.angle <= FloatT(-0.5) * pi
                || FloatT(0.5) * pi <= rect.orientation.angle)
            {
                rect.origin += rect.length_vector() + rect.width_vector();
                if (rect.orientation.angle < 0)
                    rect.orientation.angle += pi;
                else
                    rect.orientation.angle -= pi;
            }
        }

        return rect;
    }

    using Rectangle2F = Rectangle<float, 2>;
    using Rectangle2D = Rectangle<double, 2>;
    using Rectangle3F = Rectangle<float, 3>;
    using Rectangle3D = Rectangle<double, 3>;
}
