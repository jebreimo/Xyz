//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-06.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Placement.hpp"

namespace Xyz
{
    /**
     * Class for expressing rectangular cuboids.
     * @tparam T
     */
    template<std::floating_point T>
    struct OrientedCuboid
    {
        Placement<T, 3> placement;
        Vector<T, 3> size;
    };

    template<std::floating_point T>
    [[nodiscard]] bool operator==(const OrientedCuboid<T>& a,
                                  const OrientedCuboid<T>& b)
    {
        return a.placement == b.placement && a.size == b.size;
    }

    template<std::floating_point T>
    [[nodiscard]] bool operator!=(const OrientedCuboid<T>& a,
                                  const OrientedCuboid<T>& b)
    {
        return !(a == b);
    }

    template<std::floating_point T>
    [[nodiscard]]
    OrientedCuboid<T> make_centered_oriented_cuboid(const Vector<T, 3>& center,
                                                    const Vector<T, 3>& size,
                                                    const Orientation<T, 3>& orientation = {})
    {
        const auto [x, y, z] = get_vectors(orientation);
        const auto half_size = size * T(0.5);
        const auto origin = center - x * half_size[0] - y * half_size[1] - z * half_size[2];
        return OrientedCuboid<T>{
            {origin, orientation},
            size
        };
    }

    template<std::floating_point T>
    [[nodiscard]] std::tuple<Vector<T, 3>, Vector<T, 3>, Vector<T, 3>>
    get_vectors(const OrientedCuboid<T>& cuboid)
    {
        const auto [x, y, z] = get_vectors(cuboid.placement.orientation);
        const auto [l, w, h] = cuboid.size;
        return {x * l, y * w, z * h};
    }
}
