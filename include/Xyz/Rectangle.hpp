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
            : m_origin(origin), m_size(size)
        {}

        [[nodiscard]]
        Vector<T, 2> center() const
        {
            return m_origin + m_size / 2;
        }

        void set_center(const Vector<T, 2>& center)
        {
            m_origin = center - m_size / 2;
        }

        [[nodiscard]]
        const Vector<T, 2>& origin() const
        {
            return m_origin;
        }

        void set_origin(const Vector<T, 2>& origin)
        {
            m_origin = origin;
        }

        [[nodiscard]]
        const Vector<T, 2>& size() const
        {
            return m_size;
        }

        void set_size(const Vector<T, 2>& size)
        {
            m_size = size;
        }

        [[nodiscard]]
        bool is_clockwise() const
        {
            return (m_size[0] < 0) != (m_size[1] < 0);
        }

        [[nodiscard]]
        Vector<T, 2> min() const
        {
            auto [w, h] = m_size;
            if (0 <= w && 0 <= h)
                return m_origin;
            auto [x, y] = m_origin;
            if (0 <= w)
                return make_vector2(x, y + h);
            else if (0 <= h)
                return make_vector2(x + w, y);
            else
                return m_origin + m_size;
        }

        [[nodiscard]]
        Vector<T, 2> max() const
        {
            auto [w, h] = m_size;
            if (0 <= w && 0 <= h)
                return m_origin + m_size;
            auto [x, y] = m_origin;
            if (0 <= w)
                return make_vector2(x + w, y);
            else if (0 <= h)
                return make_vector2(x, y + h);
            else
                return m_origin;
        }

        /** @brief Returns the the @a nth vertex of the rectangle.
          *
          * Vertexes are listed in counter-clockwise order if the width
          * and height are both positive.
          *
          * @param n the vertex index, must be in the range 0..3
          */
        [[nodiscard]]
        Vector<T, 2> vertex(size_t n) const
        {
            switch (n & 0x3)
            {
            case 0: return m_origin;
            case 1: return m_origin + make_vector2(get<0>(m_size), T(0));
            case 2: return m_origin + m_size;
            case 3: return m_origin + make_vector2(T(0), get<1>(m_size));
            default: return Vector<T, 2>{};
            }
        }
    private:
        Vector<T, 2> m_origin;
        Vector<T, 2> m_size;
    };

    template <typename T>
    [[nodiscard]]
    Rectangle<T> make_rectangle(const Vector<T, 2>& origin,
                                const Vector<T, 2>& size)
    {
        return Rectangle<T>(origin, size);
    }

    template <typename T>
    [[nodiscard]]
    Rectangle<T> normalize(const Rectangle<T>& rectangle)
    {
        auto [x, y] = rectangle.origin();
        auto [w, h] = rectangle.size();
        if (w < 0)
        {
            x -= w;
            w = -w;
        }
        if (h < 0)
        {
            y -= h;
            h = -h;
        }
        return make_rectangle({x, y}, {w, h});
    }

    using RectangleI = Rectangle<int>;
    using RectangleF = Rectangle<float>;
    using RectangleD = Rectangle<double>;
}
