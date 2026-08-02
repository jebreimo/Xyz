//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <optional>

#include "MeshAttributeBuilder.hpp"
#include "MeshIndexBuilder.hpp"
#include "ResizableBuffer.hpp"
#include "Xyz/Vector.hpp"

namespace Xyz
{
    template <ResizableBuffer BufferType, std::integral IndexType = uint32_t>
    struct MeshBuilder
    {
        MeshAttributeBuilder<Vector3F, BufferType> position;
        MeshAttributeBuilder<Vector3F, BufferType> normal;
        std::optional<MeshAttributeBuilder<Vector3F, BufferType>> tangent;
        std::optional<MeshAttributeBuilder<Vector2F, BufferType>> texCoord;
        MeshIndexBuilder<IndexType> index;
    };
}
