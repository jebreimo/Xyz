//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MatrixTransformations.hpp"
#include "Plane.hpp"
#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief A 3D parallelogram defined by an origin point and two vectors
     * @tparam T The value type of the parallelogram.
     */
    template <typename T>
    struct Pgram3
    {
        /**
         * The origin point of the parallelogram.
         * This is normally the point with the smallest coordinates.
         */
        Vector<T, 3> origin;
        /**
         * The vector in the direction of the first edge.
         * This is normally the vector that is closest to the x-axis.
         */
        Vector<T, 3> edge0;
        /**
         * The vector in the direction of the second edge.
         * This is normally the vector that is closest to the y-axis.
         */
        Vector<T, 3> edge1;

        [[nodiscard]]
        bool is_valid(std::type_identity_t<T> margin = Margin<T>::DEFAULT) const
        {
            return !are_equal(normal(), Vector<T, 3>(), margin);
        }

        /**
         * Returns the normal vector of the plane segment.
         */
        [[nodiscard]]
        constexpr Vector<T, 3> normal() const
        {
            return cross(edge0, edge1);
        }

        /**
         * Returns the lengths of the plane segment in the direction of
         * vector1 and vector2.
         */
        [[nodiscard]]
        constexpr Vector<T, 2> size() const
        {
            return {get_length(edge0), get_length(edge1)};
        }

        /**
         * Returns the point at index i in the plane segment.
         * The points are ordered as follows:
         * 0: origin
         * 1: origin + vector1
         * 2: origin + vector1 + vector2
         * 3: origin + vector2
         */
        [[nodiscard]]
        constexpr Vector<T, 3> point(size_t i) const
        {
            switch (i % 4)
            {
            default:
            case 0: return origin;
            case 1: return origin + edge0;
            case 2: return origin + edge0 + edge1;
            case 3: return origin + edge1;
            }
        }
    };

    namespace Details
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 4, 4> get_rotation(const Pgram3<T>& p)
        {
            using V = Vector<T, 3>;

            auto z = get_ccw_angle(p.edge0, V(1, 0, 0), V(0, 0, 1));
            auto m = rotate_z(z);

            auto pt1 = m * to_hg(p.edge0);
            auto y = get_ccw_angle(from_hg(pt1), V(1, 0, 0), V(0, 1, 0));
            m = rotate_y(y) * m;

            auto pt2 = m * to_hg(p.edge1);
            auto x = get_ccw_angle(from_hg(pt2), V(0, 1, 0), V(1, 0, 0));
            return rotate_x(x) * m;
        }
    }

    template <typename T>
    [[nodiscard]]
    bool is_rectangle(const Pgram3<T>& p,
                      std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return p.is_valid(margin)
            && std::abs(dot(p.edge0, p.edge1)) <= margin;
    }

    template <typename T>
    [[nodiscard]]
    Pgram3<T> get_bounding_rectangle(const Pgram3<T>& pgram)
    {
        if (is_rectangle(pgram))
            return pgram;

        if (!pgram.is_valid())
            return Pgram3<T>();


    }

    template <std::floating_point T>
    [[nodiscard]]
    Matrix<T, 4, 4> get_clip_transform(const Pgram3<T>& p)
    {
        auto m = Details::get_rotation(p);

        auto edge1 = m * to_hg(p.edge1);
        if (edge1[0] != 0 && edge1[1] != 0)
        {
            auto shearing = Matrix<T, 4, 4>::identity();
            shearing[{0, 1}] = -edge1[0] / edge1[1];
            m = shearing * m;
        }

        auto dx = get_length(from_hg(m * to_hg(p.edge0)));
        auto dy = get_length(from_hg(m * to_hg(p.edge1)));
        auto x_scale = dx != 0 ? 1 / dx : 1;
        auto y_scale = dy != 0 ? 1 / dy : 1;
        return scale4(x_scale, y_scale, T(1)) * m * translate4(-p.origin);
    }

    template <typename T>
    [[nodiscard]]
    constexpr Plane<T> get_plane(const Pgram3<T>& rect)
    {
        return {rect.origin, rect.normal()};
    }

    using Pgram3F = Pgram3<float>;
    using Pgram3D = Pgram3<double>;
}
