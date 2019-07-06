//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MatrixClass.hpp"
#include <type_traits>

namespace Xyz
{
    template<typename T, unsigned N>
    void transpose(Matrix<T, N, N>& m)
    {
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = i + 1; j < N; ++j)
                std::swap(m[i][j], m[j][i]);
        }
    }

    template<typename T, unsigned M, unsigned N,
             typename std::enable_if<M == N, int>::type = 0>
    Matrix<T, N, M> transposed(Matrix<T, M, N>& m)
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
    Matrix<T, N, M> transposed(const Matrix<T, M, N>& m)
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
}
