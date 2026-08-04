//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "CoordinateSystem.hpp"
#include "Plane.hpp"
#include "OrientedRectangle.hpp"
#include "TransformationMatrix.hpp"
#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief A parallelogram defined by an origin point and two vectors
     * @tparam T The value type of the parallelogram.
     */
    template <typename T, unsigned N>
    struct Pgram
    {
        /**
         * The origin point of the parallelogram.
         * This is normally the point with the smallest coordinates.
         */
        Vector<T, N> origin;
        /**
         * The vector in the direction of the first edge.
         * This is normally the vector that is closest to the x-axis.
         */
        Vector<T, N> edge0;
        /**
         * The vector in the direction of the second edge.
         * This is normally the vector that is closest to the y-axis.
         */
        Vector<T, N> edge1;

        [[nodiscard]]
        bool is_valid(std::type_identity_t<T> margin = Margin<T>::DEFAULT) const
        {
            return !are_parallel(edge0, edge1, margin);
        }

        [[nodiscard]]
        T length() const
        {
            auto f1 = dot(edge0, edge1) / dot(edge0, edge0);
            return (T(1) + f1) * get_length(edge0);
        }

        [[nodiscard]]
        T width() const
        {
            auto f1 = dot(edge0, edge1) / dot(edge0, edge0);
            return get_length(edge1 - f1 * edge0);
        }

        [[nodiscard]]
        Vector<T, 2> size() const
        {
            auto f1 = dot(edge0, edge1) / dot(edge0, edge0);
            return { (T(1) + f1) * get_length(edge0), get_length(edge1 - f1 * edge0) };
        }
    };

    template <typename T, unsigned N>
    [[nodiscard]]
    bool operator==(const Pgram<T, N>& a, const Pgram<T, N>& b)
    {
        return a.origin == b.origin
            && a.edge0 == b.edge0
            && a.edge1 == b.edge1;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    bool operator!=(const Pgram<T, N>& a, const Pgram<T, N>& b)
    {
        return !(a == b);
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Pgram<T, N>& p)
    {
        return os << "{" << p.origin << ", " << p.edge0
            << ", " << p.edge1 << "}";
    }

    namespace Details
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 4, 4> get_rotation(const Pgram<T, 3>& p)
        {
            using V = Vector<T, 3>;

            auto z = get_ccw_angle(p.edge0, V(1, 0, 0), V(0, 0, 1));
            auto m = affine::rotate_z(z);

            auto pt1 = transform_vector(m, p.edge0);
            auto y = get_ccw_angle(pt1, V(1, 0, 0), V(0, 1, 0));
            m = affine::rotate_y(y) * m;

            auto pt2 = transform_vector(m, p.edge1);
            auto x = get_ccw_angle(pt2, V(0, 1, 0), V(1, 0, 0));
            return affine::rotate_x(x) * m;
        }
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, N> rel_to_abs(const Pgram<T, N>& p, Vector<std::type_identity_t<T>, 2> rel)
    {
        return p.origin + rel[0] * p.edge0 + rel[1] * p.edge1;
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    Vector<T, 2> abs_to_rel(const Pgram<T, N>& p, Vector<std::type_identity_t<T>, N> abs)
    {
        auto v = abs - p.origin;
        auto f1 = dot(v, p.edge0) / dot(p.edge0, p.edge0);
        auto f2 = dot(v, p.edge1) / dot(p.edge1, p.edge1);
        return {f1, f2};
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    bool is_rectangle(const Pgram<T, N>& p,
                      std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return p.is_valid(margin)
            && std::abs(dot(p.edge0, p.edge1)) <= margin;
    }

    template <std::floating_point T>
    [[nodiscard]]
    OrientedRectangle<T, 3>
    get_bounding_rect(const Pgram<T, 3>& pgram)
    {
        return {
            {pgram.origin, to_orientation(pgram.edge0, pgram.edge1)},
            {pgram.length(), pgram.width()}
        };
    }

    template <typename T>
    [[nodiscard]]
    Matrix<T, 4, 4> get_clip_transform(const Pgram<T, 3>& p)
    {
        auto m = Details::get_rotation(p);

        auto edge1 = transform_vector(m, p.edge1);
        if (edge1[0] != 0 && edge1[1] != 0)
        {
            auto shearing = Matrix<T, 4, 4>::identity();
            shearing[0, 1] = -edge1[0] / edge1[1];
            m = shearing * m;
        }

        auto dx = get_length(transform_vector(m, p.edge0));
        auto dy = get_length(transform_vector(m, p.edge1));
        auto x_scale = dx != 0 ? 1 / dx : 1;
        auto y_scale = dy != 0 ? 1 / dy : 1;
        return affine::scale3(x_scale, y_scale, T(1))
            * m
            * affine::translate3(-p.origin);
    }

    template <typename T, unsigned N>
    [[nodiscard]]
    constexpr Plane<T> get_plane(const Pgram<T, 3>& pgram)
    {
        return {pgram.origin, cross(pgram.edge0, pgram.edge1)};
    }

    using Pgram3F = Pgram<float, 3>;
    using Pgram3D = Pgram<double, 3>;
}
