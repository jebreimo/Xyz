//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-04.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <functional>

#include "MeshBuilder.hpp"
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
        const auto sqrt_3 = std::sqrt(3.0f);
        const auto sqrt_6 = std::sqrt(6.0f);
        const auto a = 1.0f / sqrt_3;
        const auto b = 1.0f / sqrt_6;
        const auto c = 1.0f / 3.0f;

        builder.coords.add({})
    }

}
