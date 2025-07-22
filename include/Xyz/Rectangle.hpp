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

#include "Orientation.hpp"
#include "Utilities.hpp"
#include "Placement.hpp"

namespace Xyz
{
    template <std::floating_point T, unsigned N>
    class Rectangle
    {
    public:
        using VectorT = Vector<T, N>;

        /**
         * @brief The origin of the rectangle.
         */
        Placement<T, N> placement;

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

        Rectangle() = default;

        Rectangle(const Placement<T, N>& placement,
                  T length, T width)
            : placement(placement),
              length(length),
              width(width)
        {}

        [[nodiscard]] VectorT length_vector() const
        {
            return length * get_x_vector(placement.orientation);
        }

        [[nodiscard]] VectorT width_vector() const
        {
            return width * get_y_vector(placement.orientation);
        }

        [[nodiscard]]
        constexpr VectorT operator[](size_t index) const
        {
            switch (index % 4)
            {
            default:
            case 0:
                return placement.origin;
            case 1:
                return placement.origin + length_vector();
            case 2:
                return placement.origin + length_vector() + width_vector();
            case 3:
                return placement.origin + width_vector();
            }
        }
    };

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator==(const Rectangle<T, N>& a, const Rectangle<T, N>& b)
    {
        return a.placement == b.placement
            && a.length == b.length
            && a.width == b.width;
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator!=(const Rectangle<T, N>& a, const Rectangle<T, N>& b)
    {
        return !(a == b);
    }

    template <std::floating_point T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Rectangle<T, N>& rect)
    {
        return os << '{' << rect.placement
            << ", " << Vector(rect.size) << "}";
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]] bool is_empty(const Rectangle<T, N>& rect)
    {
        return rect.length == 0 || rect.width == 0;
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    Vector<T, 2> get_center(const Rectangle<T, N>& rect)
    {
        return rect.placement.origin + (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <std::floating_point T, unsigned N>
    void set_center(Rectangle<T, N>& rect,
                    const Vector<std::type_identity_t<T>, 2>& center)
    {
        rect.placement.origin = center - (rect.length_vector() + rect.width_vector()) / T(2);
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    Rectangle<T, N> normalize(Rectangle<T, N> rect)
    {
        if (rect.length < 0)
        {
            rect.placement.origin += rect.length_vector();
            rect.length = -rect.length;
        }

        if (rect.width < 0)
        {
            rect.placement.origin += rect.width_vector();
            rect.width = -rect.width;
        }

        rect.placement.orientation = normalize(rect.placement.orientation);

        if constexpr (N == 2)
        {
            constexpr auto pi = Constants<T>::PI;

            if (rect.placement.orientation.angle <= T(-0.5) * pi
                || T(0.5) * pi <= rect.placement.orientation.angle)
            {
                rect.placement.origin += rect.length_vector() + rect.width_vector();
                if (rect.placement.orientation.angle < 0)
                    rect.placement.orientation.angle += pi;
                else
                    rect.placement.orientation.angle -= pi;
            }
        }

        return rect;
    }

    using Rectangle2F = Rectangle<float, 2>;
    using Rectangle2D = Rectangle<double, 2>;
    using Rectangle3F = Rectangle<float, 3>;
    using Rectangle3D = Rectangle<double, 3>;
}
