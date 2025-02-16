//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-06-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <array>
#include "HalfEdge.hpp"

namespace Xyz
{
    struct Face
    {
    public:
        Face()
            : vertex_ids_{INVALID_FACE_ID, INVALID_FACE_ID, INVALID_FACE_ID},
              id_(INVALID_FACE_ID)
        {}

        Face(VertexId a, VertexId b, VertexId c)
            : vertex_ids_{a, b, c},
              id_(INVALID_FACE_ID)
        {}

        Face(VertexId a, VertexId b, VertexId c, FaceId faceId)
            : vertex_ids_{a, b, c},
              id_(faceId)
        {}

        [[nodiscard]]
        VertexId operator[](size_t i) const
        {
            return vertex_ids_[i];
        }

        VertexId& operator[](size_t i)
        {
            return vertex_ids_[i];
        }

        [[nodiscard]]
        FaceId id() const
        {
            return id_;
        }

        void set_id(FaceId id)
        {
            id_ = id;
        }

        [[nodiscard]]
        VertexId* begin()
        {
            return vertex_ids_;
        }

        [[nodiscard]]
        VertexId* end()
        {
            return vertex_ids_ + 3;
        }

        [[nodiscard]]
        const VertexId* begin() const
        {
            return vertex_ids_;
        }

        [[nodiscard]]
        const VertexId* end() const
        {
            return vertex_ids_ + 3;
        }
    private:
        VertexId vertex_ids_[3];
        FaceId id_;
    };

    [[nodiscard]]
    HalfEdge get_half_edge(const Face& face, uint32_t edge_index);

    [[nodiscard]]
    std::array<HalfEdge, 3> make_half_edges(const Face& face);
}
