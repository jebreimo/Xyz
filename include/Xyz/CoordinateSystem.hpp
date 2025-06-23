//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "InvertMatrix.hpp"
#include "PlanePlaneIntersection.hpp"

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

        explicit constexpr CoordinateSystem(const Matrix<T, 4, 4>& from_world)
            : from_world_(from_world),
              to_world_(invert(from_world))
        {}

        constexpr Vector<T, 3> origin() const
        {
            return Vector<T, 3>{
                -from_world_[{0, 3}],
                -from_world_[{1, 3}],
                -from_world_[{2, 3}]
            };
        }

        constexpr Vector<T, 3> axis1() const
        {
            return Vector<T, 3>{
                from_world_[{0, 0}],
                from_world_[{0, 1}],
                from_world_[{0, 2}]
            };
        }

        constexpr Vector<T, 3> axis2() const
        {
            return Vector<T, 3>{
                from_world_[{1, 0}],
                from_world_[{1, 1}],
                from_world_[{1, 2}]
            };
        }

        constexpr Vector<T, 3> axis3() const
        {
            return Vector<T, 3>{
                from_world_[{2, 0}],
                from_world_[{2, 1}],
                from_world_[{2, 2}]
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

    template <std::floating_point T>
    constexpr bool is_valid(const CoordinateSystem<T>& cs,
                            std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return 2 <= (are_equal(cs.axis1(), Vector<T, 3>(), margin) ? 0 : 1)
            + (are_equal(cs.axis2(), Vector<T, 3>(), margin) ? 0 : 1)
            + (are_equal(cs.axis3(), Vector<T, 3>(), margin) ? 0 : 1);
    }

    template <std::floating_point T>
    CoordinateSystem<T> make_coordinate_system(const Plane<T>& plane)
    {
        auto [x, y, z] = plane.normal;
        std::optional<Line<T, 3>> line;
        if (std::abs(z) <= std::abs(x) || std::abs(z) <= std::abs(y))
        {
            line = get_intersection(plane, Plane<T>::xy());
        }
        else if (std::abs(y) <= std::abs(x))
        {
            line = get_intersection(plane, Plane<T>::xz());
        }
        else
        {
            line = get_intersection(plane, Plane<T>::yz());
        }

        if (!line)
            return CoordinateSystem<T>();

        auto axis1 = get_unit(line->vector);
        if (axis1[0] < 0
            || axis1[0] == 0 && (axis1[1] < 0
                || axis1[1] == 0 && axis1[2] < 0))
        {
            axis1 = -axis1; // Ensure positive x-axis
        }
        auto axis2 = get_unit(cross(plane.normal, axis1));
        auto axis3 = get_unit(plane.normal);

        return CoordinateSystem<T>( line->point, axis1, axis2, axis3);
    }
}
