//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <array>
#include <initializer_list>
#include <stdexcept>

namespace Xyz
{
    template<typename T, unsigned M, unsigned N>
    class Matrix
    {
    public:
        constexpr static unsigned rows()
        {
            return M;
        }

        constexpr static unsigned cols()
        {
            return N;
        }

        constexpr static unsigned size()
        {
            return M * N;
        }

        Matrix()
        {
            m_Values.fill(0);
        }

        Matrix(std::initializer_list<T> v)
        {
            if (v.size() != size())
                throw std::invalid_argument("Incorrect number of arguments.");
            auto it = v.begin();
            for (auto dst = begin(); dst != end(); ++dst, ++it)
                *dst = *it;
        }

        template<typename U>
        Matrix(U (& arr)[M * N])
        {
            for (auto dst = begin(); dst != end(); ++dst, ++arr)
                *dst = T(*arr);
        }

        template<typename U>
        explicit Matrix(const Matrix<U, M, N>& other)
        {
            auto src = other.begin();
            for (auto dst = begin(); dst != end(); ++dst, ++src)
                *dst = T(*src);
        }

        template<typename U>
        Matrix& operator=(const Matrix<U, M, N>& other)
        {
            auto src = other.begin();
            for (auto dst = begin(); dst != end(); ++dst, ++src)
                *dst = T(*src);
            return *this;
        }

        T* operator[](unsigned row)
        {
            return &m_Values[row * cols()];
        }

        const T* operator[](unsigned row) const
        {
            return &m_Values[row * cols()];
        }

        T* begin()
        {
            return m_Values.data();
        }

        T* end()
        {
            return m_Values.data() + size();
        }

        const T* begin() const
        {
            return m_Values.data();
        }

        const T* end() const
        {
            return m_Values.data() + size();
        }

        T* data()
        {
            return m_Values.data();
        }

        const T* data() const
        {
            return m_Values.data();
        }

        //static const Matrix& identity()
        //{
        //    static auto matrix = Matrix();
        //    if (matrix[N - 1][N - 1] == 0)
        //    {
        //        for (auto i = 0; i < N; ++i)
        //            matrix[i][i] = 1;
        //    }
        //    return matrix;
        //}
    private:
        std::array<T, M * N> m_Values;
    };

    template<typename T, unsigned M, unsigned N>
    const T* begin(const Matrix<T, M, N>& m)
    {
        return m.begin();
    }

    template<typename T, unsigned M, unsigned N>
    const T* end(const Matrix<T, M, N>& m)
    {
        return m.end();
    }

    template<typename T, unsigned M, unsigned N>
    T* begin(Matrix<T, M, N>& m)
    {
        return m.begin();
    }

    template<typename T, unsigned M, unsigned N>
    T* end(Matrix<T, M, N>& m)
    {
        return m.end();
    }

    template <typename T, unsigned N>
    Matrix<T, N, N> makeIdentityMatrix()
    {
        static Matrix<T, N, N> matrix;
        if (matrix[0][0] == 0)
        {
            for (auto i = 0; i < N; ++i)
                matrix[i][i] = 1;
        }
        return matrix;
    }
}
