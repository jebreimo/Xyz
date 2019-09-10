//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <array>
#include <initializer_list>
#include <stdexcept>
#include "VectorClass.hpp"

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
            for (unsigned i = 0; i < size(); ++i)
                m_Values[i] = 0;
        }

        Matrix(std::initializer_list<T> v)
        {
            if (v.size() != size())
                throw std::invalid_argument("Incorrect number of arguments.");
            auto it = v.begin();
            for (auto dst = begin(); dst != end(); ++dst, ++it)
                *dst = *it;
        }

        template <typename U>
        Matrix(U (& arr)[M * N])
        {
            for (unsigned i = 0; i < size(); ++i)
                m_Values[i] = arr[i];
        }

        Matrix(const Matrix& other)
        {
            auto src = other.data();
            for (unsigned i = 0; i < size(); ++i)
                m_Values[i] = src[i];
        }
        Matrix& operator=(const Matrix<T, M, N>& other)
        {
            auto src = other.data();
            for (unsigned i = 0; i < size(); ++i)
                m_Values[i] = src[i];
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

        T at(unsigned row, unsigned col) const
        {
            return m_Values.at(row * cols() + col);
        }

        T& at(unsigned row, unsigned col)
        {
            return m_Values.at(row * cols() + col);
        }

        Vector<T, N> row(unsigned r) const
        {
            return Vector<T, N>(&m_Values[r * cols()], N);
        }

        void setRow(unsigned r, const T* values, unsigned count)
        {
            count = count <= N ? count : N;
            std::copy(values, values + count, m_Values.data() + r * cols());
        }

        void setRow(unsigned r, const Vector<T, N>& v)
        {
            setRow(r, v.data(), v.size());
        }

        Vector<T, M> col(unsigned c) const
        {
            Vector<T, M> result;
            auto ptr = m_Values.data() + c;
            for (unsigned i = 0; i < N; ++i)
            {
                result[i] = *ptr;
                ptr += cols();
            }
            return result;
        }

        void setCol(unsigned c, const T* values, unsigned count)
        {
            count = count <= N ? count : N;
            auto ptr = m_Values + c;
            for (unsigned i = 0; i < count; ++i)
            {
                *ptr = values[i];
                ptr += cols();
            }
        }

        void setCol(unsigned c, const Vector<T, N>& v)
        {
            setCol(c, v.data(), v.size());
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

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> makeMatrixWithRows(const Vector<T, N>* rows,
                                       unsigned count)
    {
        auto n = std::min(M, count);
        Matrix<T, M, N> result;
        for (unsigned i = 0; i < n; ++i)
            result.setRow(i, rows[i]);
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> makeMatrixWithCols(const Vector<T, N>* cols,
                                       unsigned count)
    {
        auto n = std::min(M, count);
        Matrix<T, M, N> result;
        for (unsigned i = 0; i < n; ++i)
            result.setCol(i, cols[i]);
        return result;
    }

    template <unsigned K, unsigned L, typename T, unsigned M, unsigned N>
    Matrix<T, K, L> makeSubmatrix(const Matrix<T, M, N>& m,
                                  unsigned i0, unsigned j0)
    {
        static_assert(K <= M && L <= N, "The submatrix cannot be larger than the source matrix.");
        Matrix<T, K, L> result;
        for (unsigned i = 0; i < K; ++i)
        {
            auto iM = (i + i0) % M;
            for (unsigned j = 0; j < L; ++j)
            {
                auto jM = (j + j0) % N;
                result[i][j] = m[iM][jM];
            }
        }
        return result;
    }
}
