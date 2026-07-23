//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-06-05.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "FloatType.hpp"
#include "LineClipping.hpp"
#include "LineSegment.hpp"
#include "Matrix.hpp"

namespace Xyz
{
    template <typename T, unsigned N>
    class LineClipper
    {
    public:
        using FloatT = FloatType_t<T>;
        static constexpr unsigned TransformDimension = N + 1;
        using MatrixT = Matrix<FloatT, TransformDimension, TransformDimension>;

        LineClipper()
            : transform(MatrixT::identity())
        {}

        explicit LineClipper(const MatrixT& transform)
            : transform(transform)
        {}

        std::optional<LineSegment<T, N>>
        clip(const LineSegment<T, N>& segment) const
        {
            // Apply the transformation.
            auto start = transform_vector(transform, vector_cast<FloatT>(segment.start));
            auto end = transform_vector(transform, vector_cast<FloatT>(segment.end));

            // Clip the transformed segment.
            auto ts = get_clipping_positions<FloatT>(start, end);
            if (!ts)
                return std::nullopt;

            return segment.segment(ts->first, ts->second);
        }

        MatrixT transform;
    };
}
