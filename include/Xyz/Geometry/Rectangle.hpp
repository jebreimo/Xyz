//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.01.2012.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "Xyz/Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Rectangle
    {
    public:
        Rectangle()
        {}

        Rectangle(const Vector<T, 2>& origin, const Vector<T, 2>& size)
            : m_Origin(origin), m_Size(size)
        {}

        Vector<T, 2> center() const
        {
            return m_Origin + m_Size / 2;
        }

        void setCenter(const Vector<T, 2>& center)
        {
            m_Origin = center - m_Size / 2;
        }

        const Vector<T, 2>& origin() const
        {
            return m_Origin;
        }

        void setOrigin(const Vector<T, 2>& origin)
        {
            m_Origin = origin;
        }

        const Vector<T, 2>& size() const
        {
            return m_Size;
        }

        void setSize(const Vector<T, 2>& size)
        {
            m_Size = size;
        }

        Vector<T, 2> bottomLeft() const
        {
            if (0 <= getX(m_Size) && 0 <= getY(m_Size))
                return m_Origin;
            else if (0 <= getX(m_Size))
                return makeVector2(getX(m_Origin), getY(m_Origin) + getY(m_Size));
            else if (0 <= getY(m_Size))
                return makeVector2(getX(m_Origin) + getX(m_Size), getY(m_Origin));
            else
                return m_Origin + m_Size;
        }

        Vector<T, 2> topRight() const
        {
            if (0 <= getX(m_Size) && 0 <= getY(m_Size))
                return m_Origin + m_Size;
            else if (0 <= getX(m_Size))
                return makeVector2(getX(m_Origin) + getX(m_Size), getY(m_Origin));
            else if (0 <= getY(m_Size))
                return makeVector2(getX(m_Origin), getY(m_Origin) + getY(m_Size));
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
        Vector<T, 2> point(size_t n) const
        {
            switch (n & 0x3)
            {
            case 0: return m_Origin;
            case 1: return m_Origin + makeVector2(getX(m_Size), T(0));
            case 2: return m_Origin + m_Size;
            case 3: return m_Origin + makeVector2(T(0), getY(m_Size));
            default: return Vector<T, 2>{};
            }
        }
    private:
        Vector<T, 2> m_Origin;
        Vector<T, 2> m_Size;
    };

    template <typename T>
    Rectangle<T> makeRectangle(const Vector<T, 2>& origin,
                               const Vector<T, 2>& size)
    {
        return Rectangle<T>(origin, size);
    }

    template <typename T>
    Rectangle<T> normalize(const Rectangle<T>& rectangle)
    {
        auto origin = rectangle.origin();
        auto size = rectangle.size();
        if (getX(size) < 0)
        {
            getX(origin) -= getX(size);
            getX(size) = -getX(size);
        }
        if (getY(size) < 0)
        {
            getY(origin) -= getY(size);
            getY(size) = -getY(size);
        }
        return makeRectangle(origin, size);
    }
}
