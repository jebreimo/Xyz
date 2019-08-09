//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-08.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <numeric>
#include "Xyz/Fundamentals/MatrixClass.hpp"

namespace Xyz
{
    namespace Details
    {
        template <typename T, unsigned N>
        T getSubmatrixDeterminant(const Matrix <T, N, N>& m,
                                  const std::array<unsigned, 3>& columnIndices)
        {
            constexpr auto r = N - 3;
            auto a = m[r][columnIndices[0]];
            auto b = m[r][columnIndices[1]];
            auto c = m[r][columnIndices[2]];
            auto d = m[r + 1][columnIndices[0]];
            auto e = m[r + 1][columnIndices[1]];
            auto f = m[r + 1][columnIndices[2]];
            auto g = m[r + 2][columnIndices[0]];
            auto h = m[r + 2][columnIndices[1]];
            auto i = m[r + 2][columnIndices[2]];
            return a * (e * i - f * h)
                   - b * (d * i - f * g)
                   + c * (d * h - e * g);
        }

        template <typename T, unsigned M, size_t N>
        T getSubmatrixDeterminant(const Matrix<T, M, M>& m,
                                  const std::array<unsigned, N>& columnIndices)
        {
            static_assert(N > 3, "Matrix dimension must be greater than 3.");
            constexpr auto i = M - N;
            std::array<unsigned, N - 1> subIndices;
            std::copy(columnIndices.begin() + 1, columnIndices.end(),
                      subIndices.begin());
            auto determinant = T();
            for (unsigned j = 0; j < N; ++j)
            {
                if (j > 0)
                    subIndices[j - 1] = columnIndices[j - 1];
                if (m[i][j] != 0)
                {
                    auto tmp = m[i][j] * getSubmatrixDeterminant(m, subIndices);
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
    T getDeterminant(const Matrix<T, N, N>& m)
    {
        static_assert(N > 3, "Matrix dimension must be greater than 3.");
        std::array<unsigned, N - 1> subIndices;
        std::iota(subIndices.begin(), subIndices.end(), 0);
        return Details::getSubmatrixDeterminant(m, subIndices);
    }

    template <typename T>
    T getDeterminant(const Matrix<T, 3, 3>& m)
    {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
               + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2])
               + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    template <typename T>
    T getDeterminant(const Matrix<T, 2, 2>& m)
    {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
}
