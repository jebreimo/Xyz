//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "InvertMatrix.hpp"

namespace Xyz
{
    template <std::floating_point T>
    class CoordinateSystem
    {
    public:
        CoordinateSystem() = default;

        constexpr CoordinateSystem(const Vector<T, 3>& origin,
                                   const Vector<T, 3>& axis1,
                                   const Vector<T, 3>& axis2,
                                   const Vector<T, 3>& axis3)
            : CoordinateSystem(Matrix<T, 4, 4>(
                axis1[0], axis1[1], axis1[2], -origin[0],
                axis2[0], axis2[1], axis2[2], -origin[1],
                axis3[0], axis3[1], axis3[2], -origin[2],
                0, 0, 0, 1))
        {}

        explicit CoordinateSystem(const Matrix<T, 4, 4>& from_world_transform)
            : from_world_(from_world_transform),
              to_world_(invert(from_world_transform))
        {}

        constexpr const Vector<T, 3>& origin() const
        {
            return Vector<T, 3>{
                from_world_[{0, 3}],
                from_world_[{1, 3}],
                from_world_[{2, 3}]
            };
        }

        constexpr const Matrix<T, 4, 4>& from_world_transform() const
        {
            return from_world_;
        }

        constexpr const Matrix<T, 4, 4>& to_world_transform() const
        {
            return to_world_;
        }

        constexpr auto to_world_pos(const Vector<T, 3>& p) const
        {
            return from_hg(to_world_ * to_hg(p));
        }

        constexpr auto from_world_pos(const Vector<T, 3>& p) const
        {
            return from_hg(from_world_ * to_hg(p));
        }

    private:
        Matrix<T, 4, 4> from_world_;
        Matrix<T, 4, 4> to_world_;
    };
}
