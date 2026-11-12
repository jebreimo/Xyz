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
    enum class TetrahedronTextureLayout
    {
        SINGLE_TRIANGLE,
        MULTI_TRIANGLE,
        SPLIT_TRIANGLES
    };

    template <std::floating_point T>
    struct TetrahedronTextureMapping
    {
        Rectangle<T> rect = {{0, 0}, {1, 1}};
        TetrahedronTextureLayout type = TetrahedronTextureLayout::SINGLE_TRIANGLE;
    };

    template <ResizableBuffer BufferType,
    std::floating_point ValueType,
    std::integral IndexType>
    void build_tetrahedron(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                           ValueType radius = 1,
                           TetrahedronTextureMapping<ValueType> mapping = {},
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


        build_mesh(builder, Triangle<ValueType, 3>{p0, p1, p2}, {}, base_index);
        build_mesh(builder, Triangle<ValueType, 3>{p0, p1, p3}, {}, base_index);
        build_mesh(builder, Triangle<ValueType, 3>{p1, p2, p3}, {}, base_index);
        build_mesh(builder, Triangle<ValueType, 3>{p2, p0, p3}, {}, base_index);
    }

}
