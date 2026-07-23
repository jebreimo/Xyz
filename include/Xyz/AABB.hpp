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
    class AABB
    {
    public:
        Vector<T, N> min = Vector<T, N>(std::numeric_limits<T>::max());
        Vector<T, N> max = Vector<T, N>(std::numeric_limits<T>::lowest());

        AABB() noexcept = default;

        explicit AABB(const Vector<T, N>& pos)
            : min(pos), max(pos)
        {}

        AABB(const Vector<T, N>& min, const Vector<T, N>& max)
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

        friend constexpr AABB& operator+=(AABB& a, const AABB& b)
        {
            a.min = get_min(a.min, b.min);
            a.max = get_max(a.max, b.max);
            return a;
        }

        friend constexpr AABB& operator+=(AABB& a, const Vector<T, N>& b)
        {
            a.min = get_min(a.min, b);
            a.max = get_max(a.max, b);
            return a;
        }

        friend constexpr AABB operator+(const AABB& a, const AABB& b)
        {
            AABB result = a;
            result += b;
            return result;
        }

        friend constexpr AABB operator+(const AABB& a, const Vector<T, N>& b)
        {
            AABB result = a;
            result += b;
            return result;
        }

        friend constexpr AABB operator+(const Vector<T, N>& a, const AABB& b)
        {
            return b + a;
        }
    };

    template <typename T, size_t N>
    AABB<T, N> transform_aabb(const AABB<T, N>& box,
                              const Matrix<T, N + 1, N + 1>& m)
    {
        if (!box)
            return {};
        AABB<T, N> result;
        for (size_t i = 0; i < (1 << N); ++i)
        {
            Vector<T, N> corner;
            for (size_t j = 0; j < N; ++j)
                corner[j] = (i & (1 << j)) ? box.max[j] : box.min[j];
            result += transform_vector(m, corner);
        }
        return result;
    }

    using AABB2F = AABB<float, 2>;
    using AABB2D = AABB<double, 2>;
    using AABB3F = AABB<float, 3>;
    using AABB3D = AABB<double, 3>;
}
