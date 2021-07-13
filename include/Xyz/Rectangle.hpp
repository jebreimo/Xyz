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
            : m_Origin(origin), m_Size(size)
        {}

        [[nodiscard]]
        Vector<T, 2> center() const
        {
            return m_Origin + m_Size / 2;
        }

        void setCenter(const Vector<T, 2>& center)
        {
            m_Origin = center - m_Size / 2;
        }

        [[nodiscard]]
        const Vector<T, 2>& origin() const
        {
            return m_Origin;
        }

        void setOrigin(const Vector<T, 2>& origin)
        {
            m_Origin = origin;
        }

        [[nodiscard]]
        const Vector<T, 2>& size() const
        {
            return m_Size;
        }

        void setSize(const Vector<T, 2>& size)
        {
            m_Size = size;
        }

        [[nodiscard]]
        bool isClockwise() const
        {
            return (m_Size[0] < 0) != (m_Size[1] < 0);
        }

        [[nodiscard]]
        Vector<T, 2> min() const
        {
            auto [w, h] = m_Size;
            if (0 <= w && 0 <= h)
                return m_Origin;
            auto [x, y] = m_Origin;
            if (0 <= w)
                return makeVector2(x, y + h);
            else if (0 <= h)
                return makeVector2(x + w, y);
            else
                return m_Origin + m_Size;
        }

        [[nodiscard]]
        Vector<T, 2> max() const
        {
            auto [w, h] = m_Size;
            if (0 <= w && 0 <= h)
                return m_Origin + m_Size;
            auto [x, y] = m_Origin;
            if (0 <= w)
                return makeVector2(x + w, y);
            else if (0 <= h)
                return makeVector2(x, y + h);
            else
                return m_Origin;
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
            case 0: return m_Origin;
            case 1: return m_Origin + makeVector2(get<0>(m_Size), T(0));
            case 2: return m_Origin + m_Size;
            case 3: return m_Origin + makeVector2(T(0), get<1>(m_Size));
            default: return Vector<T, 2>{};
            }
        }
    private:
        Vector<T, 2> m_Origin;
        Vector<T, 2> m_Size;
    };

    template <typename T>
    [[nodiscard]]
    Rectangle<T> makeRectangle(const Vector<T, 2>& origin,
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
        return makeRectangle({x, y}, {w, h});
    }
}
