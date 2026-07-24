//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-23.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Matrix.hpp"
#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief An axis-aligned bounding box.
     * @tparam T The coordinate type, e.g. float or double.
     * @tparam N The coordinate dimension.
     */
    template <typename T, size_t N>
    class BBox
    {
    public:
        Vector<T, N> min = Vector<T, N>(std::numeric_limits<T>::max());
        Vector<T, N> max = Vector<T, N>(std::numeric_limits<T>::lowest());

        BBox() noexcept = default;

        explicit BBox(const Vector<T, N>& pos)
            : min(pos), max(pos)
        {}

        BBox(const Vector<T, N>& min, const Vector<T, N>& max)
            : min(min), max(max)
        {}

        constexpr explicit operator bool() const
        {
            for (size_t i = 0; i < N; ++i)
            {
                if (min[i] > max[i])
                    return false;
            }
            return true;
        }

        friend constexpr BBox& operator+=(BBox& a, const BBox& b)
        {
            a.min = get_min(a.min, b.min);
            a.max = get_max(a.max, b.max);
            return a;
        }

        friend constexpr BBox& operator+=(BBox& a, const Vector<T, N>& b)
        {
            a.min = get_min(a.min, b);
            a.max = get_max(a.max, b);
            return a;
        }

        friend constexpr BBox operator+(const BBox& a, const BBox& b)
        {
            BBox result = a;
            result += b;
            return result;
        }

        friend constexpr BBox operator+(const BBox& a, const Vector<T, N>& b)
        {
            BBox result = a;
            result += b;
            return result;
        }

        friend constexpr BBox operator+(const Vector<T, N>& a, const BBox& b)
        {
            return b + a;
        }
    };

    /**
     * Returns the axis-aligned bounding box of @a box after it has been
     * transformed.
     */
    template <typename T, size_t N>
    BBox<T, N> transform_bbox(const BBox<T, N>& box,
                              const Matrix<T, N + 1, N + 1>& m)
    {
        if (!box)
            return {};
        BBox<T, N> result;
        for (size_t i = 0; i < (1 << N); ++i)
        {
            Vector<T, N> corner;
            for (size_t j = 0; j < N; ++j)
                corner[j] = (i & (1 << j)) ? box.max[j] : box.min[j];
            result += transform_vector(m, corner);
        }
        return result;
    }

    /**
     * Returns the axis-aligned bounding box of @a box after it has been
     * transformed without considering the w component.
     *
     * This is an optimized version of transform_bbox, but must only be
     * used when the final row of matrix @a m consists of 0s followed by
     * a single 1.
     */
    template <typename T, size_t N>
    BBox<T, N> transform_bbox_no_w(const BBox<T, N>& box,
                                   const Matrix<T, N + 1, N + 1>& m)
    {
        if (!box)
            return {};
        BBox<T, N> result;
        for (size_t i = 0; i < (1 << N); ++i)
        {
            Vector<T, N> corner;
            for (size_t j = 0; j < N; ++j)
                corner[j] = (i & (1 << j)) ? box.max[j] : box.min[j];
            result += transform_vector_no_w(m, corner);
        }
        return result;
    }

    using BBox2F = BBox<float, 2>;
    using BBox2D = BBox<double, 2>;
    using BBox3F = BBox<float, 3>;
    using BBox3D = BBox<double, 3>;
}
