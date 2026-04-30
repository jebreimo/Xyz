//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.01.2012.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Rectangle
    {
    public:
        Vector<T, 2> origin;
        Vector<T, 2> size;

        Rectangle() = default;

        Rectangle(const Vector<T, 2>& origin, const Vector<T, 2>& size)
            : origin(origin), size(size)
        {}

        /**
         * @note the points are only clockwise if both values of size are
         *  either positive or negative.
         */
        [[nodiscard]]
        constexpr Vector<T, 2> operator[](size_t index) const
        {
            switch (index % 4)
            {
            default:
            case 0:
                return origin;
            case 1:
                return origin + Vector<T, 2>(size.x(), 0);
            case 2:
                return origin + size;
            case 3:
                return origin + Vector<T, 2>(0, size.y());
            }
        }
    };

    template <typename T>
    [[nodiscard]]
    bool operator==(const Rectangle<T>& a, const Rectangle<T>& b)
    {
        return a.origin == b.origin && a.size == b.size;
    }

    template <typename T>
    [[nodiscard]]
    bool operator!=(const Rectangle<T>& a, const Rectangle<T>& b)
    {
        return a.origin != b.origin || a.size != b.size;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Rectangle<T>& rect)
    {
        return os << '{' << rect.origin << ", " << rect.size << "}";
    }

    template <typename T>
    [[nodiscard]]
    bool is_empty(const Rectangle<T>& rect)
    {
        return rect.size[0] == 0 || rect.size[1] == 0;
    }

    template <typename T>
    [[nodiscard]]
    bool is_null(const Rectangle<T>& rect)
    {
        return rect.size[0] == 0 && rect.size[1] == 0;
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 2> get_min(const Rectangle<T>& rect)
    {
        return get_min(rect.origin, rect.origin + rect.size);
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 2> get_max(const Rectangle<T>& rect)
    {
        return get_max(rect.origin, rect.origin + rect.size);
    }

    template <typename T>
    [[nodiscard]]
    constexpr Vector<T, 2> get_center(const Rectangle<T>& rect)
    {
        return rect.origin + rect.size / 2;
    }

    template <typename T>
    void set_center(Rectangle<T>& rect, const Vector<std::type_identity_t<T>, 2>& center)
    {
        rect.origin = center - rect.size / 2;
    }

    template <typename T>
    [[nodiscard]]
    Rectangle<T> normalize(const Rectangle<T>& rectangle)
    {
        auto [x, y] = rectangle.origin;
        auto [w, h] = rectangle.size;
        if (w < 0)
        {
            x += w;
            w = -w;
        }
        if (h < 0)
        {
            y += h;
            h = -h;
        }
        return Rectangle<T>({x, y}, {w, h});
    }

    using RectangleI = Rectangle<int>;
    using RectangleF = Rectangle<float>;
    using RectangleD = Rectangle<double>;
}
