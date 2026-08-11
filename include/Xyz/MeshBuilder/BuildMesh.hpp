//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include <functional>

#include "MeshBuilder.hpp"
#include "Xyz/OrientedCuboid.hpp"
#include "Xyz/Pgram.hpp"
#include "Xyz/Rectangle.hpp"

namespace Xyz
{
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType = uint32_t>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const Pgram<ValueType, 3>& pgram,
                    const Rectangle<ValueType>& tex_rect = {},
                    IndexType base_index = 0)
    {
        const auto& v0 = pgram.edge0;
        const auto& v1 = pgram.edge1;

        builder.coords.reserve(builder.coords.size() + 4);
        builder.coords.add(pgram.origin);
        builder.coords.add(pgram.origin + v0);
        builder.coords.add(pgram.origin + v0 + v1);
        builder.coords.add(pgram.origin + v1);

        if (builder.normals)
        {
            builder.normals->add_n(normalize(cross(v0, v1)), 4);
        }

        if (builder.tangents)
        {
            // The tangent points along increasing u and the bitangent along
            // increasing v. The tangent's fourth component is the handedness,
            // i.e. the sign that makes bitangent == w * cross(normal,
            // tangent). cross(normal, v0) always points to the v1 side of the
            // surface, so the handedness is negative only when the texture
            // rectangle mirrors exactly one of the two axes.
            const auto u = tex_rect.size.x() < 0 ? ValueType(-1) : ValueType(1);
            const auto v = tex_rect.size.y() < 0 ? ValueType(-1) : ValueType(1);
            builder.tangents->add_n(make_vector4(normalize(v0) * u, u * v), 4);
        }

        if (builder.tex_coords)
        {
            builder.tex_coords->reserve(builder.tex_coords->size() + 4);
            const auto [tv0, tv1] = get_vectors(tex_rect);
            builder.tex_coords->add(tex_rect.origin);
            builder.tex_coords->add(tex_rect.origin + tv0);
            builder.tex_coords->add(tex_rect.origin + tex_rect.size);
            builder.tex_coords->add(tex_rect.origin + tv1);
        }

        builder.indexes.reserve(builder.indexes.size() + 6);
        builder.indexes.add(base_index + 0, base_index + 2, base_index + 3);
        builder.indexes.add(base_index + 0, base_index + 1, base_index + 2);
    }

    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType = uint32_t>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const OrientedRectangle<ValueType, 3>& rect,
                    const Rectangle<ValueType>& tex_rect = {},
                    IndexType base_index = 0)
    {
        const auto [x, y] = get_vectors(rect);
        build_mesh(builder,
                   Pgram<ValueType, 3>(rect.placement.origin, x, y),
                   tex_rect,
                   base_index);
    }

    /**
     * @brief Builds a mesh for an OrientedCuboid.
     * @param builder The mesh builder to use.
     * @param cuboid The OrientedCuboid to build a mesh for.
     * @param tex_rect_func A function that returns the texture rectangle for each
     *        face of the cuboid. The function is called with the face index (0-5).
     *        If not provided, the texture coordinates will be set to (0, 0).
     *        The order of the faces is: left, front, right, back, top, bottom.
     * @param base_index The base index to use for the mesh indices.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType = uint32_t>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const OrientedCuboid<ValueType>& cuboid,
                    std::function<Rectangle<ValueType>(int)> tex_rect_func = {},
                    IndexType base_index = 0)
    {
        using P = Pgram<ValueType, 3>;
        using R = Rectangle<ValueType>;
        const auto [x, y, z] = get_vectors(cuboid);
        build_mesh(builder,
                   P(cuboid.placement.origin + y, -y, z),
                   tex_rect_func ? tex_rect_func(0) : R{},
                   base_index);
        build_mesh(builder,
                   P(cuboid.placement.origin, x, z),
                   tex_rect_func ? tex_rect_func(1) : R{},
                   base_index + 4);
        build_mesh(builder,
                   P(cuboid.placement.origin + x, y, z),
                   tex_rect_func ? tex_rect_func(2) : R{},
                   base_index + 8);
        build_mesh(builder,
                   P(cuboid.placement.origin + x + y, -x, z),
                   tex_rect_func ? tex_rect_func(3) : R{},
                   base_index + 12);
        build_mesh(builder,
                   P(cuboid.placement.origin + z, x, y),
                   tex_rect_func ? tex_rect_func(4) : R{},
                   base_index + 16);
        build_mesh(builder,
                   P(cuboid.placement.origin + y, x, -y),
                   tex_rect_func ? tex_rect_func(5) : R{},
                   base_index + 20);
    }
}
