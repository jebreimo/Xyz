//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>

#include "Xyz/Pentagon.hpp"
#include "MeshBuilder.hpp"

namespace Xyz
{
    /**
     * @brief Adds @a pentagon to @a builder as five vertexes and three
     *  triangles.
     *
     * The pentagon must be planar and convex. It is triangulated as a fan
     * from its first point.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const Pentagon<ValueType, 3>& pentagon,
                    const Pentagon<ValueType, 2>& tex_pentagon = {},
                    std::type_identity_t<IndexType> base_index = {})
    {
        builder.coords.reserve(builder.coords.size() + 5);
        for (const auto& p : pentagon.points)
            builder.coords.add(p);

        if (builder.normals)
        {
            builder.normals->add_n(get_normal(pentagon), 5);
        }

        if (builder.tangents)
        {
            // Same convention as for triangles: the tangent follows the
            // first edge, and the handedness is always positive.
            builder.tangents->add_n(make_vector4(normalize(pentagon[1] - pentagon[0]), ValueType(1)), 5);
        }

        if (builder.tex_coords)
        {
            builder.tex_coords->reserve(builder.tex_coords->size() + 5);
            for (const auto& p : tex_pentagon.points)
                builder.tex_coords->add(p);
        }

        builder.indexes.reserve(builder.indexes.size() + 9);
        builder.indexes.add(base_index + 0, base_index + 1, base_index + 2);
        builder.indexes.add(base_index + 0, base_index + 2, base_index + 3);
        builder.indexes.add(base_index + 0, base_index + 3, base_index + 4);
    }
}
