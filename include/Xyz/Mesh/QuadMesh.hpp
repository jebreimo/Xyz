//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>

#include "Xyz/Pgram.hpp"
#include "Xyz/Rectangle.hpp"
#include "TriangleMesh.hpp"

namespace Xyz
{
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const Pgram<ValueType, 3>& pgram,
                    const Rectangle<ValueType>& tex_rect = {},
                    std::type_identity_t<IndexType> base_index = {})
    {
        const auto [x, y] = get_vectors(pgram);
        builder.coords.reserve(builder.coords.size() + 4);
        builder.coords.add(pgram.origin);
        builder.coords.add(pgram.origin + x);
        builder.coords.add(pgram.origin + x + y);
        builder.coords.add(pgram.origin + y);

        if (builder.normals)
        {
            builder.normals->add_n(normalize(cross(x, y)), 4);
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
            builder.tangents->add_n(make_vector4(normalize(x) * u, u * v), 4);
        }

        if (builder.tex_coords)
        {
            builder.tex_coords->reserve(builder.tex_coords->size() + 4);
            const auto [tx, ty] = get_vectors(tex_rect);
            builder.tex_coords->add(tex_rect.origin);
            builder.tex_coords->add(tex_rect.origin + tx);
            builder.tex_coords->add(tex_rect.origin + tex_rect.size);
            builder.tex_coords->add(tex_rect.origin + ty);
        }

        builder.indexes.reserve(builder.indexes.size() + 6);
        builder.indexes.add(base_index + 0, base_index + 2, base_index + 3);
        builder.indexes.add(base_index + 0, base_index + 1, base_index + 2);
    }

    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const OrientedRectangle<ValueType, 3>& rect,
                    const Rectangle<ValueType>& tex_rect = {},
                    std::type_identity_t<IndexType> base_index = {})
    {
        const auto [x, y] = get_vectors(rect);
        build_mesh(builder,
                   Pgram<ValueType, 3>(rect.placement.origin, x, y),
                   tex_rect,
                   base_index);
    }
}
