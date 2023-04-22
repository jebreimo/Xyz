//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-06-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <cstdint>

namespace Xyz
{
    using VertexId = uint32_t;
    using FaceId = uint32_t;
    using HalfEdgeId = uint32_t;

    constexpr VertexId INVALID_VERTEX_ID = ~VertexId(0);
    constexpr FaceId INVALID_FACE_ID = ~FaceId(0);
    constexpr HalfEdgeId INVALID_HALF_EDGE_ID = ~HalfEdgeId(0);

    struct HalfEdge
    {
    public:
        HalfEdge()
            : HalfEdge(INVALID_VERTEX_ID, INVALID_VERTEX_ID,
                       INVALID_HALF_EDGE_ID)
        {}

        HalfEdge(VertexId a, VertexId b, HalfEdgeId id)
            : vertex_ids_{a, b},
              id_(id)
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
        HalfEdgeId id() const
        {
            return id_;
        }

        void set_id(HalfEdgeId id)
        {
            id_ = id;
        }
    private:
        VertexId vertex_ids_[2];
        HalfEdgeId id_;
    };

    inline bool operator==(const HalfEdge& a, const HalfEdge& b)
    {
        return a[0] == b[0]
               && a[1] == b[1]
               && a.id() == b.id();
    }

    inline bool operator!=(const HalfEdge& a, const HalfEdge& b)
    {
        return !(a == b);
    }

    inline bool operator<(const HalfEdge& a, const HalfEdge& b)
    {
        auto a0 = a[0], a1 = a[1];
        if (a0 > a1)
            std::swap(a0, a1);
        auto b0 = b[0], b1 = b[1];
        if (b0 > b1)
            std::swap(b0, b1);
        if (a0 != b0)
            return a0 < b0;
        if (a1 != b1)
            return a1 < b1;
        return a[0] < b[0];
    }

    inline bool are_equal(const HalfEdge& a, const HalfEdge& b)
    {
        return a[0] == b[0] && a[1] == b[1];
    }

    inline bool are_opposites(const HalfEdge& a, const HalfEdge& b)
    {
        return a[0] == b[1] && a[1] == b[0];
    }
}
