//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "InvertMatrix.hpp"
#include "Orientation.hpp"
#include "PlanePlaneIntersection.hpp"

namespace Xyz
{
    template <std::floating_point T>
    class CoordinateSystem
    {
    public:
        CoordinateSystem() = default;

        constexpr CoordinateSystem(const Vector<T, 3>& origin,
                                   const Vector<T, 3>& x_axis,
                                   const Vector<T, 3>& y_axis,
                                   const Vector<T, 3>& z_axis)
            : CoordinateSystem(Matrix<T, 4, 4>(
                x_axis[0], y_axis[0], z_axis[0], -origin[0],
                x_axis[1], y_axis[1], z_axis[1], -origin[1],
                x_axis[2], y_axis[2], z_axis[2], -origin[2],
                0, 0, 0, 1))
        {}

        explicit CoordinateSystem(const Orientation<T, 3>& orientation,
                                  const Vector<T, 3>& origin = {0, 0, 0})
            : CoordinateSystem(affine::to_matrix(orientation, origin))
        {}

        explicit constexpr CoordinateSystem(const Matrix<T, 4, 4>& from_cs)
            : from_cs_(from_cs),
              to_cs_(invert(from_cs))
        {}

        constexpr Vector<T, 3> origin() const
        {
            return {
                -from_cs_[{0, 3}],
                -from_cs_[{1, 3}],
                -from_cs_[{2, 3}]
            };
        }

        constexpr Vector<T, 3> x_axis() const
        {
            return {
                from_cs_[{0, 0}],
                from_cs_[{1, 0}],
                from_cs_[{2, 0}]
            };
        }

        constexpr Vector<T, 3> y_axis() const
        {
            return {
                from_cs_[{0, 1}],
                from_cs_[{1, 1}],
                from_cs_[{2, 1}]
            };
        }

        constexpr Vector<T, 3> z_axis() const
        {
            return {
                from_cs_[{0, 2}],
                from_cs_[{1, 2}],
                from_cs_[{2, 2}]
            };
        }

        constexpr const Matrix<T, 4, 4>& from_cs_transform() const
        {
            return from_cs_;
        }

        constexpr const Matrix<T, 4, 4>& to_cs_transform() const
        {
            return to_cs_;
        }

        constexpr Vector<T, 3> to_cs(const Vector<T, 3>& p) const
        {
            return drop_back(to_cs_ * to_hg(p));
        }

        constexpr Vector<T, 3> from_cs(const Vector<T, 3>& p) const
        {
            return drop_back(from_cs_ * to_hg(p));
        }

        constexpr Vector<T, 2> to_cs_xy(const Vector<T, 3>& p) const
        {
            return resize<2>(to_cs_ * to_hg(p));
        }

        constexpr Vector<T, 3> from_cs_xy(const Vector<T, 2>& p) const
        {
            return drop_back(from_cs_ * Vector<T, 4>(p[0], p[1], 0, 1));
        }

    private:
        Matrix<T, 4, 4> from_cs_ = Matrix<T, 4, 4>::identity();
        Matrix<T, 4, 4> to_cs_ = Matrix<T, 4, 4>::identity();
    };

    /**
     * @brief Checks if the coordinate system is valid.
     *
     * A coordinate system is valid if no pair of axes are parallel.
     */
    template <std::floating_point T>
    constexpr bool is_valid(const CoordinateSystem<T>& cs,
                            std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        constexpr Vector<T, 3> null;
        return !are_equal(cross(cs.x_axis(), cs.y_axis()), null, margin)
            && !are_equal(cross(cs.x_axis(), cs.z_axis()), null, margin)
            && !are_equal(cross(cs.y_axis(), cs.z_axis()), null, margin);
    }

    template <std::floating_point T>
    std::optional<CoordinateSystem<T>> make_coordinate_system(const Plane<T>& plane)
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
            return std::nullopt; // No intersection with any of the planes

        auto axis1 = normalize(line->vector);
        if (axis1[0] < 0
            || (axis1[0] == 0 && (axis1[1] < 0
                || (axis1[1] == 0 && axis1[2] < 0))))
        {
            axis1 = -axis1; // Ensure positive x-axis
        }
        auto axis3 = normalize(plane.normal);
        auto axis2 = normalize(cross(axis3, axis1));

        return CoordinateSystem<T>(line->point, axis1, axis2, axis3);
    }
}
