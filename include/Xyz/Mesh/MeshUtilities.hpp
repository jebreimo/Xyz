//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-10-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <concepts>
#include <cstdint>
#include <span>
#include <vector>

namespace Xyz
{
    namespace Details
    {
        template <std::integral IndexType>
        uint64_t to_edge(IndexType a, IndexType b)
        {
            static_assert(sizeof(IndexType) <= sizeof(uint64_t) / 2, "IndexType is too large");
            if (a > b)
                std::swap(a, b);
            return static_cast<uint64_t>(a) << 32 | static_cast<uint64_t>(b);
        }

        template <std::integral IndexType>
        std::vector<IndexType> make_line_indexes(std::vector<uint64_t> edges)
        {
            std::ranges::sort(edges);
            edges.erase(std::ranges::unique(edges).begin(), edges.end());

            std::vector<IndexType> lines;
            for (const auto segment : edges)
            {
                lines.push_back(static_cast<IndexType>(segment >> 32));
                lines.push_back(static_cast<IndexType>(segment & 0xFFFFFFFF));
            }
            return lines;
        }
    }

    template <std::integral IndexType>
    std::vector<IndexType>
    triangles_to_line_indexes(std::span<const IndexType> indexes)
    {
        using Details::to_edge;
        std::vector<uint64_t> edges;
        for (size_t i = 0; i + 2 < indexes.size(); i += 3)
        {
            edges.push_back(to_edge(indexes[i], indexes[i + 1]));
            edges.push_back(to_edge(indexes[i], indexes[i + 2]));
            edges.push_back(to_edge(indexes[i + 1], indexes[i + 2]));
        }

        return Details::make_line_indexes<IndexType>(std::move(edges));
    }

    template <std::integral IndexType>
    std::vector<IndexType>
    triangle_fan_to_line_indexes(std::span<const IndexType> indexes)
    {
        if (indexes.size() < 3)
            return {};

        using Details::to_edge;
        std::vector<uint64_t> edges;
        edges.push_back(to_edge(indexes[0], indexes[1]));
        for (size_t i = 1; i + 1 < indexes.size(); i += 1)
        {
            edges.push_back(to_edge(indexes[0], indexes[i + 1]));
            edges.push_back(to_edge(indexes[i], indexes[i + 1]));
        }

        return Details::make_line_indexes<IndexType>(std::move(edges));
    }

    template <std::integral IndexType>
    std::vector<IndexType>
    triangle_strip_to_line_indexes(std::span<const IndexType> indexes)
    {
        if (indexes.size() < 3)
            return {};

        using Details::to_edge;
        std::vector<uint64_t> edges;
        edges.push_back(to_edge(indexes[0], indexes[1]));
        for (size_t i = 0; i + 2 < indexes.size(); i += 1)
        {
            edges.push_back(to_edge(indexes[i], indexes[i + 2]));
            edges.push_back(to_edge(indexes[i + 1], indexes[i + 2]));
        }

        return Details::make_line_indexes<IndexType>(std::move(edges));
    }
}
