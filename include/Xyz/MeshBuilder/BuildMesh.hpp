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
        const auto& vector0 = pgram.edge0;
        const auto& vector1 = pgram.edge1;

        builder.coords.reserve(builder.coords.size() + 4);
        builder.coords.add(pgram.origin);
        builder.coords.add(pgram.origin + vector0);
        builder.coords.add(pgram.origin + vector0 + vector1);
        builder.coords.add(pgram.origin + vector1);

        if (builder.normals)
        {
            builder.normals->add_n(cross(vector0, vector1), 4);
        }

        if (builder.tangents)
        {
            const auto tangent = make_vector4(vector0, 1);
            builder.tangents->add_n(tangent, 4);
        }

        if (builder.tex_coords)
        {
            builder.tex_coords->reserve(builder.tex_coords->size() + 4);
            builder.tex_coords->add(tex_rect.origin);
            builder.tex_coords->add(tex_rect.origin + Vector<ValueType, 2>{tex_rect.size.x(), 0});
            builder.tex_coords->add(tex_rect.origin + tex_rect.size);
            builder.tex_coords->add(tex_rect.origin + Vector<ValueType, 2>{0, tex_rect.size.y()});
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
}
