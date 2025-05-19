//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    class PlaneSegment
    {
    public:
        PlaneSegment() = default;

        PlaneSegment(const Vector<T, 3>& origin,
                     const Vector<T, 3>& vector1,
                     const Vector<T, 3>& vector2)
            : origin(origin),
              vector1(vector1),
              vector2(vector2)
        {}

        [[nodiscard]]
        bool is_valid() const
        {
            return get_length_squared(normal()) != 0;
        }

        /**
         * Returns the normal vector of the plane segment.
         */
        [[nodiscard]]
        constexpr Vector<T, 3> normal() const
        {
            return cross(vector1, vector2);
        }

        /**
         * Returns the lengths of the plane segment in the direction of
         * vector1 and vector2.
         */
        [[nodiscard]]
        constexpr Vector<T, 2> size() const
        {
            return {get_length(vector1), get_length(vector2)};
        }

        /**
         * Returns the point at index i in the plane segment.
         * The points are ordered as follows:
         * 0: origin
         * 1: origin + vector1
         * 2: origin + vector1 + vector2
         * 3: origin + vector2
         */
        [[nodiscard]]
        constexpr Vector<T, 3> point(size_t i) const
        {
            switch (i % 4)
            {
            default:
            case 0: return origin;
            case 1: return origin + vector1;
            case 2: return origin + vector1 + vector2;
            case 3: return origin + vector2;
            }
        }

        Vector<T, 3> origin;
        Vector<T, 3> vector1;
        Vector<T, 3> vector2;
    };
}
