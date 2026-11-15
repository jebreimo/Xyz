//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>

#include "MeshBuilder.hpp"
#include "Xyz/Triangle.hpp"

namespace Xyz
{
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const Triangle<ValueType, 3>& triangle,
                    const Triangle<ValueType, 2>& tex_triangle = {},
                    std::type_identity_t<IndexType> base_index = {})
    {
        const auto [x, y] = get_vectors(triangle);
        builder.coords.reserve(builder.coords.size() + 3);
        builder.coords.add(triangle[0]);
        builder.coords.add(triangle[1]);
        builder.coords.add(triangle[2]);

        if (builder.normals)
        {
            builder.normals->add_n(get_normal(triangle), 3);
        }

        if (builder.tangents)
        {
            // The tangent points along increasing u and the bitangent along
            // increasing v. The tangent's fourth component is the handedness,
            // i.e. the sign that makes bitangent == w * cross(normal,
            // tangent). cross(normal, v0) always points to the v1 side of the
            // surface, so the handedness is negative only when the texture
            // rectangle mirrors exactly one of the two axes.
            builder.tangents->add_n(make_vector4(normalize(triangle[1] - triangle[0]), ValueType(1)), 3);
        }

        if (builder.tex_coords)
        {
            builder.tex_coords->reserve(builder.tex_coords->size() + 3);
            const auto [tx, ty] = get_vectors(tex_triangle);
            builder.tex_coords->add(tex_triangle.origin);
            builder.tex_coords->add(tex_triangle.origin + tx);
            builder.tex_coords->add(tex_triangle.origin + tex_triangle.size);
            builder.tex_coords->add(tex_triangle.origin + ty);
        }

        builder.indexes.reserve(builder.indexes.size() + 3);
        builder.indexes.add(base_index + 0, base_index + 2, base_index + 3);
    }
}
