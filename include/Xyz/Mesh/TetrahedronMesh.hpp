//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-04.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "TriangleMesh.hpp"
#include "Xyz/Orientation.hpp"

namespace Xyz
{
    template <std::floating_point T>
    struct OrientedRegularPolyhedron
    {
        Vector<T, 3> center;
        T radius = 1;
        Orientation<T, 3> orientation = {};
    };

    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_tetrahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                           const OrientedRegularPolyhedron<ValueType>& polyhedron = {},
                           std::span<Triangle<ValueType, 2>> tex_maps = {},
                           std::type_identity_t<IndexType> base_index = {})
    {
        const auto radius = polyhedron.radius;
        const auto sqrt_2 = std::sqrt(ValueType(2));
        const auto sqrt_3 = std::sqrt(ValueType(3));

        const auto a = radius * sqrt_2 / sqrt_3;
        const auto b = radius * sqrt_2 / ValueType(3);
        const auto c = radius * ValueType(1) / ValueType(3);
        const auto d = ValueType(2) * b;

        const auto rotation = linear::to_matrix(polyhedron.orientation);
        auto transform = [&](const Vector<ValueType, 3>& p)
        {
            return polyhedron.center + rotation * p;
        };

        const auto p0 = transform({-a, -b, -c});
        const auto p1 = transform({a, -b, -c});
        const auto p2 = transform({ValueType(0), d, -c});
        const auto p3 = transform({ValueType(0), ValueType(0), radius});

        auto tex_tri = [&tex_maps](size_t i)
        {
            if (tex_maps.empty())
                return Triangle<ValueType, 2>{};
            return tex_maps[i % tex_maps.size()];
        };

        using Tri3 = Triangle<ValueType, 3>;

        build_mesh(builder, Tri3{p0, p2, p1}, tex_tri(0), base_index);
        build_mesh(builder, Tri3{p0, p1, p3}, tex_tri(1), base_index + 3);
        build_mesh(builder, Tri3{p1, p2, p3}, tex_tri(2), base_index + 6);
        build_mesh(builder, Tri3{p2, p0, p3}, tex_tri(3), base_index + 9);
    }
}
