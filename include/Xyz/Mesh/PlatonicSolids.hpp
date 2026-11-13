//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-04.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <functional>

#include "BuildMesh.hpp"
#include "Xyz/Rectangle.hpp"

namespace Xyz
{
    template <ResizableBuffer BufferType,
    std::floating_point ValueType,
    std::integral IndexType>
    void build_tetrahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                           ValueType radius = 1,
                           std::span<Triangle<ValueType, 2>> tex_maps = {},
                           std::type_identity_t<IndexType> base_index = {})
    {
        const auto sqrt_3 = std::sqrt(ValueType(3));
        const auto sqrt_2 = std::sqrt(ValueType(2));

        const auto a = radius * sqrt_2 / sqrt_3;
        const auto b = radius * sqrt_2 / ValueType(3);
        const auto c = radius * ValueType(1) / ValueType(3);
        const auto d = ValueType(2) * b;

        const auto p0 = Vector<ValueType, 3>{-a, -b, -c};
        const auto p1 = Vector<ValueType, 3>{a, -b, -c};
        const auto p2 = Vector<ValueType, 3>{ValueType(0), d, -c};
        const auto p3 = Vector<ValueType, 3>{ValueType(0), ValueType(0), radius};

        auto tex_tri = [&tex_maps](size_t i)
        {
            if (tex_maps.empty())
                return Triangle<ValueType, 2>{};
            return tex_maps[i % tex_maps.size()];
        };

        using Tri3 = Triangle<ValueType, 3>;

        build_mesh(builder, Tri3{p0, p1, p2}, tex_tri(0), base_index);
        build_mesh(builder, Tri3{p0, p1, p3}, tex_tri(1), base_index);
        build_mesh(builder, Tri3{p1, p2, p3}, tex_tri(2), base_index);
        build_mesh(builder, Tri3{p2, p0, p3}, tex_tri(3), base_index);
    }
}
