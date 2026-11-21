//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "TriangleMesh.hpp"
#include "Xyz/OrientedRegularPolyhedron.hpp"

namespace Xyz
{
    /**
     * @brief Adds the eight faces of a regular octahedron to @a builder.
     *
     * Before the orientation is applied, the octahedron's vertexes lie on
     * the positive and negative x, y and z axes. The first four faces are
     * the ones that meet at the top vertex (positive z), the last four
     * the ones that meet at the bottom vertex (negative z). Each face gets
     * its own three vertexes, and the triangles are wound counterclockwise
     * as seen from outside.
     *
     * @param tex_maps The texture coordinates of each face. If there are
     *  fewer maps than faces, the maps are repeated in order.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_octahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                          const OrientedRegularPolyhedron<ValueType>& polyhedron = {},
                          std::span<Triangle<ValueType, 2>> tex_maps = {},
                          std::type_identity_t<IndexType> base_index = {})
    {
        const auto r = polyhedron.radius;
        const auto zero = ValueType(0);

        const auto rotation = linear::to_matrix(polyhedron.orientation);
        auto transform = [&](const Vector<ValueType, 3>& p)
        {
            return polyhedron.center + rotation * p;
        };

        const auto px = transform({r, zero, zero});
        const auto nx = transform({-r, zero, zero});
        const auto py = transform({zero, r, zero});
        const auto ny = transform({zero, -r, zero});
        const auto pz = transform({zero, zero, r});
        const auto nz = transform({zero, zero, -r});

        auto tex_tri = [&tex_maps](size_t i)
        {
            if (tex_maps.empty())
                return Triangle<ValueType, 2>{};
            return tex_maps[i % tex_maps.size()];
        };

        using Tri3 = Triangle<ValueType, 3>;

        const Tri3 faces[] = {
            {px, py, pz},
            {py, nx, pz},
            {nx, ny, pz},
            {ny, px, pz},
            {py, px, nz},
            {nx, py, nz},
            {ny, nx, nz},
            {px, ny, nz}
        };

        for (size_t i = 0; i < std::size(faces); ++i)
        {
            build_mesh(builder, faces[i], tex_tri(i),
                       base_index + IndexType(i * 3));
        }
    }
}
