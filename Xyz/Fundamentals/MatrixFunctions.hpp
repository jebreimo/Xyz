//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "MatrixClass.hpp"

namespace Xyz {

    template<typename T, unsigned N>
    void transpose(Matrix<T, N>& m)
    {
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = i + 1; j < N; ++j)
                std::swap(m[i][j], m[j][i]);
        }
    }

    template<typename T, unsigned N>
    Matrix<T, N> transposed(Matrix<T, N>& m)
    {
        Matrix<T, N> t;
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = 0u; j < N; ++j)
                t[i][j] = m[j][i];
        }
        return t;
    }

    template<typename T, typename U, unsigned N>
    auto multiplyTransposed(const Matrix<T, N>& a, const Matrix<U, N>& b)
    -> Matrix<decltype(T() * U()), N>
    {
        typedef decltype(T() * U()) ResType;
        Matrix<ResType, N> c;
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = 0u; j < N; ++j)
            {
                ResType v = 0;
                for (auto k = 0u; k < N; ++k)
                    v += a[i][k] * b[j][k];
                c[i][j] = v;
            }
        }
        return c;
    }

}
