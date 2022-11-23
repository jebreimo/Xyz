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
                : m_origin(origin)
        {
            set_row(m_from_world, 0, axis1 / get_length_squared(axis1));
            set_row(m_from_world, 1, axis2 / get_length_squared(axis2));
            set_row(m_from_world, 2, axis3 / get_length_squared(axis3));
            m_to_world = invert(m_from_world);
        }

        CoordinateSystem(const Vector<T, 3>& origin,
                         const Matrix<T, 3, 3>& from_world_transform)
                : m_origin(origin),
                  m_to_world(invert(from_world_transform)),
                  m_from_world(from_world_transform)
        {}

        constexpr const Vector<T, 3>& origin() const
        {
            return m_origin;
        }

        constexpr const Matrix<T, 3, 3>& from_world_transform() const
        {
            return m_from_world;
        }

        constexpr const Matrix<T, 3, 3>& to_world_transform() const
        {
            return m_to_world;
        }

        template <typename U>
        constexpr auto to_world_pos(const Vector<U, 3>& p) const
        {
            return m_to_world * p + m_origin;
        }

        template <typename U>
        constexpr auto from_world_pos(const Vector<U, 3>& p) const
        {
            return m_from_world * (p - m_origin);
        }
    private:
        Vector<T, 3> m_origin;
        Matrix<T, 3, 3> m_to_world;
        Matrix<T, 3, 3> m_from_world;
    };
}
