//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.01.2012.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <vector>
#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Rectangle
    {
    public:
        Rectangle() = default;

        Rectangle(const Vector<T, 2>& origin, const Vector<T, 2>& size)
            : origin(origin), size(size)
        {}

        Vector<T, 2> origin;
        Vector<T, 2> size;
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
        return a.origin != b.origin || a.size == b.size;
    }

    template <typename T>
    [[nodiscard]]
    std::ostream& operator<<(std::ostream& os, const Rectangle<T>& rect)
    {
        return os << '{' << rect.origin << ", " << rect.size << "}";
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_bottom_left(const Rectangle<T>& rect)
    {
        return rect.origin;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_bottom_right(const Rectangle<T>& rect)
    {
        return rect.origin + Vector<T, 2>{rect.size[0], 0};
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_top_left(const Rectangle<T>& rect)
    {
        return rect.origin + Vector<T, 2>{0, rect.size[1]};
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_top_right(const Rectangle<T>& rect)
    {
        return rect.origin + rect.size;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_min(const Rectangle<T>& rect)
    {
        auto [w, h] = rect.size;
        if (0 <= w && 0 <= h)
            return rect.origin;
        auto [x, y] = rect.origin;
        if (0 <= w)
            return {x, y + h};
        else if (0 <= h)
            return {x + w, y};
        else
            return rect.origin + rect.size;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_max(const Rectangle<T>& rect)
    {
        auto [w, h] = rect.size;
        if (0 <= w && 0 <= h)
            return rect.origin + rect.size;
        auto [x, y] = rect.origin;
        if (0 <= w)
            return {x + w, y};
        else if (0 <= h)
            return {x, y + h};
        else
            return rect.origin;
    }

    template <typename T>
    [[nodiscard]]
    Vector<T, 2> get_center(const Rectangle<T>& rect)
    {
        return rect.origin + rect.size / 2;
    }

    template <typename T>
    void set_center(Rectangle<T>& rect, const Vector<T, 2>& center)
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
