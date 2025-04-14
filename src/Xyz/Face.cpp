//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-06-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Face.hpp"
#include <array>

namespace Xyz
{
    HalfEdge get_half_edge(const Face& face, uint32_t edge_index)
    {
        switch (edge_index)
        {
        case 0:
            return {face[0], face[1], face.id() * 3};
        case 1:
            return {face[1], face[2], face.id() * 3 + 1};
        case 2:
            return {face[2], face[0], face.id() * 3 + 2};
        default:
            return {};
        }
    }

    std::array<HalfEdge, 3> make_half_edges(const Face& face)
    {
        return {{{face[0], face[1], face.id() * 3},
                 {face[1], face[2], face.id() * 3 + 1},
                 {face[2], face[0], face.id() * 3 + 2}}};
    }
}
