//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MatrixClass.hpp"
#include <numeric>
#include <type_traits>

namespace Xyz
{
    template<typename T, unsigned N>
    void transposeAssign(Matrix<T, N, N>& m)
    {
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = i + 1; j < N; ++j)
                std::swap(m[i][j], m[j][i]);
        }
    }

    template<typename T, unsigned M, unsigned N,
             typename std::enable_if<M == N, int>::type = 0>
    Matrix<T, N, M> transpose(const Matrix<T, M, N>& m)
    {
        Matrix<T, N, M> result;
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = 0u; j < N; ++j)
                result[i][j] = m[j][i];
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N,
              typename std::enable_if<M != N, int>::type = 0>
    Matrix<T, N, M> transpose(const Matrix<T, M, N>& m)
    {
        Matrix<T, N, M> result;
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = 0u; j < M; ++j)
                result[i][j] = m[j][i];
        }
        return result;
    }

    template<typename T, unsigned M, unsigned N, unsigned O>
    Matrix<T, M, O> multiplyTransposed(const Matrix<T, M, N>& a,
                                       const Matrix<T, O, N>& b)
    {
        Matrix<T, M, O> result;
        for (auto i = 0u; i < M; ++i)
        {
            for (auto j = 0u; j < N; ++j)
            {
                T v = 0;
                for (auto k = 0u; k < N; ++k)
                    v += a[i][k] * b[j][k];
                result[i][j] = v;
            }
        }
        return result;
    }

    template <typename T, unsigned N>
    std::pair<Matrix<T, N, N>, Matrix<T, N, N>> decomposeLU(Matrix<T, N, N> a)
    {
        Matrix<T, N, N> u = {};
        auto l = makeIdentityMatrix<T, N>();
        for (unsigned k = 0; k < N; ++k)
        {
            u.at[k][k] = a[k][k];
            for (unsigned i = k + 1; i < N; ++i)
            {
                l[i][k] = a[i][k] / u[k][k];
                u[k][i] = a[k][i];
            }
            for (unsigned i = k + 1; i < N; ++i)
                for (unsigned j = k + 1; j < N; ++j)
                    a[i][j] = a[i][j] - l[i][k] * u[k][j];
        }
        return {l, u};
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

    template <typename T, unsigned M>
    T getSubmatrixDeterminant(const Matrix <T, M, M>& m,
                              const std::array<unsigned, 3>& columnIndices)
    {
        constexpr auto r = M - 3;
        auto a = m[r][columnIndices[0]];
        auto b = m[r][columnIndices[1]];
        auto c = m[r][columnIndices[2]];
        auto d = m[r + 1][columnIndices[0]];
        auto e = m[r + 1][columnIndices[1]];
        auto f = m[r + 1][columnIndices[2]];
        auto g = m[r + 2][columnIndices[0]];
        auto h = m[r + 2][columnIndices[1]];
        auto i = m[r + 2][columnIndices[2]];
        return a * (e*i - f*h) - b * (d*i - f*g) + c * (d*h - e*g);
    }

    template <typename T, unsigned N>
    T getDeterminant(const Matrix <T, N, N>& m)
    {
        static_assert(N >= 3, "Matrix dimension must be greater than 3.");
        std::array<unsigned, N> subIndices;
        std::iota(subIndices.begin(), subIndices.end(), 0);
        return getSubmatrixDeterminant(m, subIndices);
    }

    template <typename T>
    T getDeterminant(const Matrix <T, 3, 3>& m)
    {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
               + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2])
               + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }
}
