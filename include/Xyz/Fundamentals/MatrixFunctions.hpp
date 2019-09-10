//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include <numeric>
#include <type_traits>
#include <Xyz/Utilities/XyzException.hpp>
#include "MatrixClass.hpp"

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

    template <typename T, unsigned M, unsigned N>
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
    bool areEquivalent(const Matrix<T, N, N>& a, const Matrix<T, N, N>& b,
                       double epsilon = 1e-12)
    {
        for (unsigned i = 0; i < N; ++i)
            for (unsigned j = 0; j < N; ++j)
                if (std::abs(a[i][j] - b[i][j]) > epsilon)
                    return false;
        return true;
    }
}
