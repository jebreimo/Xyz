//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "PentagonMesh.hpp"
#include "Xyz/Constants.hpp"
#include "Xyz/OrientedRegularPolyhedron.hpp"

namespace Xyz
{
    /**
     * @brief Adds the twelve faces of a regular dodecahedron to @a builder.
     *
     * Before the orientation is applied, the dodecahedron has a top face
     * centered on the positive z-axis and a bottom face centered on the
     * negative z-axis. The twenty vertexes form four rings of five: the
     * top face, an upper middle ring, a lower middle ring and the bottom
     * face. The two upper rings each have a vertex in the xz-plane on the
     * positive x side, and the two lower rings are rotated 36 degrees
     * relative to them.
     *
     * The faces are added in this order: the top face, the five faces
     * that touch the top face, the five faces that touch the bottom face,
     * and the bottom face. Each face gets its own five vertexes, and is
     * split into three triangles that are wound counterclockwise as seen
     * from outside.
     *
     * The five points of each face, and thus the five points of its
     * texture map, are in counterclockwise order as seen from outside.
     * For the faces that touch the top face, the first two points are
     * the top-left and the middle-left points. For the faces that touch
     * the bottom face, the first point is the top point. For the top
     * and bottom faces, the first point is the one in the direction of
     * the positive x-axis.
     *
     * @param tex_maps The texture coordinates of each face. If there are
     *  fewer maps than faces, the maps are repeated in order.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_dodecahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                            const OrientedRegularPolyhedron<ValueType>& polyhedron = {},
                            std::span<Pentagon<ValueType, 2>> tex_maps = {},
                            std::type_identity_t<IndexType> base_index = {})
    {
        using Vec3 = Vector<ValueType, 3>;
        using Pent3 = Pentagon<ValueType, 3>;

        const auto r = polyhedron.radius;
        const auto sqrt_5 = std::sqrt(ValueType(5));
        // The vertexes of the top and bottom faces lie on circles with
        // radius outer_r at z = ±outer_z. The vertexes of the middle rings
        // lie on circles with radius inner_r (the golden ratio times
        // outer_r) at z = ±inner_z.
        const auto outer_r = r * std::sqrt((10 - 2 * sqrt_5) / 15);
        const auto outer_z = r * std::sqrt((5 + 2 * sqrt_5) / 15);
        const auto inner_r = r * std::sqrt((10 + 2 * sqrt_5) / 15);
        const auto inner_z = r * std::sqrt((5 - 2 * sqrt_5) / 15);
        constexpr auto step = ValueType(2) * Constants<ValueType>::PI / 5;

        const auto rotation = linear::to_matrix(polyhedron.orientation);
        auto transform = [&](const Vec3& p)
        {
            return polyhedron.center + rotation * p;
        };

        Vec3 top[5];
        Vec3 upper[5];
        Vec3 lower[5];
        Vec3 bottom[5];
        for (size_t i = 0; i < 5; ++i)
        {
            const auto a = ValueType(i) * step;
            const auto b = a + step / 2;
            const auto cos_a = std::cos(a);
            const auto sin_a = std::sin(a);
            const auto cos_b = std::cos(b);
            const auto sin_b = std::sin(b);
            top[i] = transform({outer_r * cos_a, outer_r * sin_a, outer_z});
            upper[i] = transform({inner_r * cos_a, inner_r * sin_a, inner_z});
            lower[i] = transform({inner_r * cos_b, inner_r * sin_b, -inner_z});
            bottom[i] = transform({outer_r * cos_b, outer_r * sin_b, -outer_z});
        }

        auto tex_pent = [&tex_maps](size_t i)
        {
            if (tex_maps.empty())
                return Pentagon<ValueType, 2>{};
            return tex_maps[i % tex_maps.size()];
        };

        size_t face = 0;
        auto add_face = [&](const Pent3& pentagon)
        {
            build_mesh(builder, pentagon, tex_pent(face),
                       base_index + IndexType(face * 5));
            ++face;
        };

        add_face({top[0], top[1], top[2], top[3], top[4]});

        for (size_t i = 0; i < 5; ++i)
        {
            const auto j = (i + 1) % 5;
            add_face({top[i], upper[i], lower[i], upper[j], top[j]});
        }

        for (size_t i = 0; i < 5; ++i)
        {
            const auto j = (i + 1) % 5;
            add_face({upper[j], lower[i], bottom[i], bottom[j], lower[j]});
        }

        add_face({bottom[0], bottom[4], bottom[3], bottom[2], bottom[1]});
    }
}
