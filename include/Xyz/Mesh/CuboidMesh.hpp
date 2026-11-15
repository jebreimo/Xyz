//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>

#include "Xyz/OrientedCuboid.hpp"
#include "Xyz/Pgram.hpp"
#include "Xyz/Rectangle.hpp"
#include "QuadMesh.hpp"

namespace Xyz
{
    /**
     * @brief Builds a mesh for an OrientedCuboid.
     * @note Unless the cuboid is rotated, the front face is the one whose
     *       normal points in the direction of the x-axis, and the top face's
     *       normal points in the direction of the z-axis.
     * @param builder The mesh builder to use.
     * @param cuboid The OrientedCuboid to build a mesh for.
     * @param tex_rects A span of texture rectangles for each face of the cuboid.
     *        If not provided, the texture coordinates will be set to (0, 0).
     *        The order of the faces is: left, front, right, back, top, bottom.
     *        If fewer than 6 rectangles are provided, they will be repeated in order.
     * @param base_index The base index to use for the mesh indices.
     */
    template <ResizableBuffer BufferType,
        std::floating_point ValueType,
        std::integral IndexType>
    void build_mesh(MeshBuilder<BufferType, ValueType, IndexType>& builder,
                    const OrientedCuboid<ValueType>& cuboid,
                    std::span<Rectangle<ValueType>> tex_rects = {},
                    std::type_identity_t<IndexType> base_index = {})
    {
        using P = Pgram<ValueType, 3>;
        using R = Rectangle<ValueType>;
        const auto [x, y, z] = get_vectors(cuboid);

        auto tex_rect = [&tex_rects](size_t i)
        {
            if (tex_rects.empty())
                return R{};
            return tex_rects[i % tex_rects.size()];
        };

        // Left face (-y)
        build_mesh(builder,
                   P(cuboid.placement.origin, x, z),
                   tex_rect(0),
                   base_index + 0);
        // Front face (+x)
        build_mesh(builder,
                   P(cuboid.placement.origin + x, y, z),
                   tex_rect(1),
                   base_index + 4);
        // Right face (+y)
        build_mesh(builder,
                   P(cuboid.placement.origin + x + y, -x, z),
                   tex_rect(2),
                   base_index + 8);
        // Back face (-x)
        build_mesh(builder,
                   P(cuboid.placement.origin + y, -y, z),
                   tex_rect(3),
                   base_index + 12);
        // Bottom face (-z)
        build_mesh(builder,
                   P(cuboid.placement.origin + y, x, -y),
                   tex_rect(4),
                   base_index + 16);
        // Top face (+z)
        build_mesh(builder,
                   P(cuboid.placement.origin + z, x, y),
                   tex_rect(5),
                   base_index + 20);
    }
}
