//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "InvertMatrix.hpp"

namespace Xyz
{
    template <typename T>
    class CoordinateSystem
    {
    public:
        CoordinateSystem() = default;

        CoordinateSystem(const Vector<T, 3>& origin,
                         const Vector<T, 3>& axis1,
                         const Vector<T, 3>& axis2,
                         const Vector<T, 3>& axis3)
                : m_Origin(origin)
        {
            m_FromWorld.setRow(0, axis1 / getLengthSquared(axis1));
            m_FromWorld.setRow(1, axis2 / getLengthSquared(axis2));
            m_FromWorld.setRow(2, axis3 / getLengthSquared(axis3));
            m_ToWorld = invert(m_FromWorld);
        }

        CoordinateSystem(const Vector<T, 3>& origin,
                         const Matrix<T, 3, 3>& fromWorldTransform)
                : m_Origin(origin),
                  m_ToWorld(invert(fromWorldTransform)),
                  m_FromWorld(fromWorldTransform)
        {}

        constexpr const Vector<T, 3>& origin() const
        {
            return m_Origin;
        }

        constexpr const Matrix<T, 3, 3>& fromWorldTransform() const
        {
            return m_FromWorld;
        }

        constexpr const Matrix<T, 3, 3>& toWorldTransform() const
        {
            return m_ToWorld;
        }

        template <typename U>
        constexpr auto toWorldPos(const Vector<U, 3>& p) const
        {
            return m_ToWorld * p + m_Origin;
        }

        template <typename U>
        constexpr auto fromWorldPos(const Vector<U, 3>& p) const
        {
            return m_FromWorld * (p - m_Origin);
        }
    private:
        Vector<T, 3> m_Origin;
        Matrix<T, 3, 3> m_ToWorld;
        Matrix<T, 3, 3> m_FromWorld;
    };
}
