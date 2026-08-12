//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <optional>

#include "MeshAttributeBuilder.hpp"
#include "MeshIndexBuilder.hpp"
#include "Xyz/Vector.hpp"

namespace Xyz
{
    template <
        ResizableBuffer BufferType,
        std::floating_point ValueTypeT,
        std::integral IndexTypeT = uint32_t>
    struct MeshBuilder
    {
        using ValueType = ValueTypeT;
        using IndexType = IndexTypeT;

        using Vector2 = Vector<ValueType, 2>;
        using Vector3 = Vector<ValueType, 3>;
        using Vector4 = Vector<ValueType, 4>;

        MeshIndexBuilder<IndexTypeT> indexes;
        MeshAttributeBuilder<Vector3, BufferType> coords;
        std::optional<MeshAttributeBuilder<Vector3, BufferType>> normals = {};
        std::optional<MeshAttributeBuilder<Vector4, BufferType>> tangents = {};
        std::optional<MeshAttributeBuilder<Vector2, BufferType>> tex_coords = {};
    };
}
