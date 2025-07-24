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
        /**
         * @brief The origin of the rectangle.
         */
        Placement<T, N> placement;

        Vector<T, 2> size;

        Rectangle() = default;

        Rectangle(const Placement<T, N>& placement,
                  const Vector<T, 2>& size)
            : placement(placement),
              size(size)
        {}

        [[nodiscard]] Vector<T, N> length_vector() const
        {
            return size.x() * get_x_vector(placement.orientation);
        }

        [[nodiscard]] Vector<T, N> width_vector() const
        {
            return size.y() * get_y_vector(placement.orientation);
        }

        [[nodiscard]]
        constexpr Vector<T, N> operator[](size_t index) const
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
            && a.size == b.size;
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
        return os << '{' << rect.placement << ", " << rect.size << "}";
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]] bool is_empty(const Rectangle<T, N>& rect)
    {
        return rect.size.x() == 0 || rect.size.y() == 0;
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
        if (rect.size.x() < 0)
        {
            rect.placement.origin += rect.length_vector();
            rect.size.x() = -rect.size.x();
        }

        if (rect.size.y() < 0)
        {
            rect.placement.origin += rect.width_vector();
            rect.size.y() = -rect.size.y();
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
