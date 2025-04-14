//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-08.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <array>
#include <numeric>
#include "Matrix.hpp"

namespace Xyz
{
    namespace Details
    {
        template <typename T, unsigned N>
        T get_submatrix_determinant(
            const Matrix<T, N, N>& m,
            const std::array<unsigned, 3>& column_indices)
        {
            constexpr auto r = N - 3;
            auto a = m[{r, column_indices[0]}];
            auto b = m[{r, column_indices[1]}];
            auto c = m[{r, column_indices[2]}];
            auto d = m[{r + 1, column_indices[0]}];
            auto e = m[{r + 1, column_indices[1]}];
            auto f = m[{r + 1, column_indices[2]}];
            auto g = m[{r + 2, column_indices[0]}];
            auto h = m[{r + 2, column_indices[1]}];
            auto i = m[{r + 2, column_indices[2]}];
            return a * (e * i - f * h)
                   - b * (d * i - f * g)
                   + c * (d * h - e * g);
        }

        template <typename T, unsigned M, size_t N>
        T get_submatrix_determinant(
            const Matrix<T, M, M>& m,
            const std::array<unsigned, N>& column_indices)
        {
            static_assert(N > 3, "Matrix dimension must be greater than 3.");
            constexpr auto i = unsigned(M - N);
            std::array<unsigned, N - 1> sub_indices;
            std::copy(column_indices.begin() + 1, column_indices.end(),
                      sub_indices.begin());
            auto determinant = T();
            for (unsigned j = 0; j < N; ++j)
            {
                if (j > 0)
                    sub_indices[j - 1] = column_indices[j - 1];
                if (m[{i, j}] != 0)
                {
                    auto tmp = m[{i, j}] * get_submatrix_determinant(m, sub_indices);
                    if (j % 2 == 0)
                        determinant += tmp;
                    else
                        determinant -= tmp;
                }
            }
            return determinant;
        }
    }

    template <typename T, unsigned N>
    T get_determinant(const Matrix<T, N, N>& m)
    {
        static_assert(N > 3, "Matrix dimension must be greater than 3.");
        std::array<unsigned, N> sub_indices;
        std::iota(sub_indices.begin(), sub_indices.end(), 0);
        return Details::get_submatrix_determinant(m, sub_indices);
    }

    template <typename T>
    T get_determinant(const Matrix<T, 3, 3>& m)
    {
        return m[{0, 0}] * (m[{1, 1}] * m[{2, 2}] - m[{1, 2}] * m[{2, 1}])
               + m[{0, 1}] * (m[{1, 2}] * m[{2, 0}] - m[{1, 0}] * m[{2, 2}])
               + m[{0, 2}] * (m[{1, 0}] * m[{2, 1}] - m[{1, 1}] * m[{2, 0}]);
    }

    template <typename T>
    T get_determinant(const Matrix<T, 2, 2>& m)
    {
        return m[{0, 0}] * m[{1, 1}] - m[{0, 1}] * m[{1, 0}];
    }
}
