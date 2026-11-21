//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "TriangleMesh.hpp"
#include "Xyz/Constants.hpp"
#include "Xyz/OrientedRegularPolyhedron.hpp"

namespace Xyz
{
    /**
     * @brief Adds the twenty faces of a regular icosahedron to @a builder.
     *
     * Before the orientation is applied, the icosahedron has a top vertex
     * on the positive z-axis and a bottom vertex on the negative z-axis.
     * The remaining ten vertexes form two rings of five. The upper ring
     * has a vertex in the xz-plane on the positive x side, and the lower
     * ring is rotated 36 degrees relative to the upper ring.
     *
     * The faces are added in this order: the five faces that meet at the
     * top vertex, the ten faces of the band between the two rings, and
     * the five faces that meet at the bottom vertex. Each face gets its own
     * three vertexes, and the triangles are wound counterclockwise as seen
     * from outside.
     *
     * @param tex_maps The texture coordinates of each face. If there are
     *  fewer maps than faces, the maps are repeated in order.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_icosahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                           const OrientedRegularPolyhedron<ValueType>& polyhedron = {},
                           std::span<Triangle<ValueType, 2>> tex_maps = {},
                           std::type_identity_t<IndexType> base_index = {})
    {
        using Vec3 = Vector<ValueType, 3>;
        using Tri3 = Triangle<ValueType, 3>;

        const auto r = polyhedron.radius;
        const auto sqrt_5 = std::sqrt(ValueType(5));
        // The rings lie at z = ±r/√5, with a ring radius of 2r/√5.
        const auto ring_z = r / sqrt_5;
        const auto ring_r = ValueType(2) * r / sqrt_5;
        constexpr auto step = ValueType(2) * Constants<ValueType>::PI / 5;

        const auto rotation = linear::to_matrix(polyhedron.orientation);
        auto transform = [&](const Vec3& p)
        {
            return polyhedron.center + rotation * p;
        };

        const auto top = transform({ValueType(0), ValueType(0), r});
        const auto bottom = transform({ValueType(0), ValueType(0), -r});

        Vec3 upper[5];
        Vec3 lower[5];
        for (size_t i = 0; i < 5; ++i)
        {
            const auto a = ValueType(i) * step;
            const auto b = a + step / 2;
            upper[i] = transform({ring_r * std::cos(a), ring_r * std::sin(a), ring_z});
            lower[i] = transform({ring_r * std::cos(b), ring_r * std::sin(b), -ring_z});
        }

        auto tex_tri = [&tex_maps](size_t i)
        {
            if (tex_maps.empty())
                return Triangle<ValueType, 2>{};
            return tex_maps[i % tex_maps.size()];
        };

        size_t face = 0;
        auto add_face = [&](const Vec3& a, const Vec3& b, const Vec3& c)
        {
            build_mesh(builder, Tri3{a, b, c}, tex_tri(face),
                       base_index + IndexType(face * 3));
            ++face;
        };

        for (size_t i = 0; i < 5; ++i)
            add_face(upper[i], upper[(i + 1) % 5], top);

        for (size_t i = 0; i < 5; ++i)
        {
            const auto j = (i + 1) % 5;
            add_face(upper[i], lower[i], upper[j]);
            add_face(lower[i], lower[j], upper[j]);
        }

        for (size_t i = 0; i < 5; ++i)
            add_face(lower[(i + 1) % 5], lower[i], bottom);
    }
}
