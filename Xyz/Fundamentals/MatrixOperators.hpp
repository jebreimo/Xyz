//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <ostream>
#include "MatrixClass.hpp"
#include "VectorOperators.hpp"

namespace Xyz
{
    template<typename T, unsigned N, unsigned M>
    bool operator==(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto aData = a.data();
        auto bData = b.data();
        for (auto i = 0u; i < M * N; ++i)
        {
            if (aData[i] != bData[i])
                return false;
        }
        return true;
    }

    template<typename T, unsigned M, unsigned N>
    bool operator!=(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        return !(a == b);
    }

    template<typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator+=(Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto dataA = a.data();
        auto dataB = b.data();
        for (auto i = 0u; i < M * N; ++i)
            dataA[i] += dataB[i];
        return a;
    }

    template<typename T, unsigned M, unsigned N>
    Matrix<T, M, N> operator+(const Matrix<T, M, N>& a,
                              const Matrix<T, M, N>& b)
    {
        Matrix<T, M, N> c(a);
        return c += b;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator-=(Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto dataA = a.data();
        auto dataB = b.data();
        for (auto i = 0u; i < M * N; ++i)
            dataA[i] -= dataB[i];
        return a;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> operator-(const Matrix<T, M, N>& a,
                              const Matrix<T, M, N>& b)
    {
        Matrix<T, M, N> c(a);
        return c -= b;
    }

    template <typename T, unsigned M, unsigned N, unsigned O>
    Matrix<T, M, N> operator*(const Matrix<T, M, N>& a,
                              const Matrix<T, N, O>& b)
    {
        Matrix<T, M, O> result;
        for (auto i = 0u; i < M; ++i)
        {
            for (auto j = 0u; j < O; ++j)
            {
                T v = 0;
                for (auto k = 0u; k < N; ++k)
                    v += a[i][k] * b[k][j];
                result[i][j] = v;
            }
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N, unsigned O>
    Matrix<T, M, O>& operator*=(Matrix<T, M, N>& a, const Matrix<T, N, O>& b)
    {
        return a = a * b;
    }

    template <typename T, unsigned M, unsigned N>
    Vector<T, M> operator*(const Matrix<T, M, N>& m, const Vector<T, N>& v)
    {
        Vector<T, M> result;
        for (auto i = 0u; i < M; ++i)
        {
            T value = 0;
            for (auto j = 0u; j < N; ++j)
                value += m[i][j] * v[j];
            result[i] = value;
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    Vector<T, N> operator*(const Vector<T, M>& v, const Matrix<T, M, N>& m)
    {
        Vector<T, N> result;
        for (auto i = 0u; i < N; ++i)
        {
            T value = 0;
            for (auto j = 0u; j < M; ++j)
                value += v[j] * m[j][i];
            result[i] = value;
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& m)
    {
        os << m[0][0];
        for (auto j = 1u; j < N; ++j)
            os << " " << m[0][j];
        for (auto i = 1u; i < M; ++i)
        {
            os << " |";
            for (auto j = 0u; j < N; ++j)
                os << " " << m[i][j];
        }
        return os;
    }
}
