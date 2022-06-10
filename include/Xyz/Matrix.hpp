//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Vector.hpp"

namespace Xyz
{
    template <typename T, unsigned M, unsigned N>
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

        static constexpr Matrix identity()
        {
            static_assert(M == N);
            Matrix result;
            for (unsigned i = 0; i < M; ++i)
                result[i][i] = 1;
            return result;
        }

        constexpr Matrix()
            : m_values()
        {}

        Matrix(std::initializer_list<T> v)
        {
            if (v.size() != size())
                XYZ_THROW("Incorrect number of arguments.");
            std::copy(v.begin(), v.end(), begin());
        }

        explicit Matrix(T (&other)[ M * N])
        {
            std::copy(std::begin(other), std::end(other), begin());
        }

        template <typename U>
        Matrix(const U* values, size_t count)
        {
            if (count != size())
                XYZ_THROW("Incorrect number of arguments.");

            std::copy(values, values + count, begin());
        }

        Matrix(const Matrix& other)
        {
            std::copy(other.begin(), other.end(), begin());
        }

        Matrix& operator=(const Matrix& other)
        {
            if (&other == this)
                return *this;
            std::copy(other.begin(), other.end(), begin());
            return *this;
        }

        constexpr T* operator[](unsigned row)
        {
            return &m_values[row * cols()];
        }

        constexpr const T* operator[](unsigned row) const
        {
            return &m_values[row * cols()];
        }

        Vector<T, N> row(unsigned r) const
        {
            return Vector<T, N>(&m_values[r * cols()], N);
        }

        void set_row(unsigned row, const Vector<T, N>& v)
        {
            std::copy(v.begin(), v.end(), m_values + row * cols());
        }

        void set_row(unsigned row, const T* values, unsigned count)
        {
            if (count != N)
                XYZ_THROW("Incorrect number of columns.");
            std::copy(values, values + count, m_values.data() + row * cols());
        }

        Vector<T, M> col(unsigned c) const
        {
            Vector<T, M> result;
            auto ptr = m_values.data() + c;
            for (unsigned i = 0; i < M; ++i)
            {
                result[i] = *ptr;
                ptr += cols();
            }
            return result;
        }

        void set_col(unsigned c, const Vector<T, M>& v)
        {
            auto ptr = m_values + c;
            for (unsigned i = 0; i < M; ++i)
            {
                *ptr = v[i];
                ptr += cols();
            }
        }

        void set_col(unsigned c, const T* values, unsigned count)
        {
            if (count != M)
                XYZ_THROW("Incorrect number of columns.");
            auto ptr = m_values + c;
            for (unsigned i = 0; i < M; ++i)
            {
                *ptr = values[i];
                ptr += cols();
            }
        }

        constexpr T* begin()
        {
            return m_values;
        }

        constexpr T* end()
        {
            return m_values + size();
        }

        constexpr const T* begin() const
        {
            return m_values;
        }

        constexpr const T* end() const
        {
            return m_values + size();
        }

        constexpr T* data()
        {
            return m_values;
        }

        constexpr const T* data() const
        {
            return m_values;
        }
    private:
        T m_values[M * N];
    };

    template <typename T, unsigned N>
    Matrix<T, N, N> make_identity_matrix()
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
    Matrix<T, M, N> make_matrix_with_rows(const Vector<T, N>* rows,
                                          unsigned count)
    {
        auto n = std::min(M, count);
        Matrix<T, M, N> result;
        for (unsigned i = 0; i < n; ++i)
            result.set_row(i, rows[i]);
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> make_matrix_with_cols(const Vector<T, N>* cols,
                                          unsigned count)
    {
        auto n = std::min(M, count);
        Matrix<T, M, N> result;
        for (unsigned i = 0; i < n; ++i)
            result.set_col(i, cols[i]);
        return result;
    }

    template <unsigned K, unsigned L, typename T, unsigned M, unsigned N>
    Matrix<T, K, L> make_submatrix(const Matrix<T, M, N>& m,
                                   unsigned i0, unsigned j0)
    {
        static_assert(K <= M && L <= N,
                      "The submatrix cannot be larger than the source matrix.");
        Matrix<T, K, L> result;
        for (unsigned i = 0; i < K; ++i)
        {
            auto i_m = (i + i0) % M;
            for (unsigned j = 0; j < L; ++j)
            {
                auto j_m = (j + j0) % N;
                result[i][j] = m[i_m][j_m];
            }
        }
        return result;
    }

    template <typename T, unsigned N, unsigned M>
    bool operator==(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto a_data = a.data();
        auto b_data = b.data();
        for (auto i = 0u; i < M * N; ++i)
        {
            if (a_data[i] != b_data[i])
                return false;
        }
        return true;
    }

    template <typename T, unsigned M, unsigned N>
    bool operator!=(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        return !(a == b);
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator+=(Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto a_data = a.data();
        auto b_data = b.data();
        for (auto i = 0u; i < M * N; ++i)
            a_data[i] += b_data[i];
        return a;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> operator+(const Matrix<T, M, N>& a,
                              const Matrix<T, M, N>& b)
    {
        Matrix<T, M, N> c(a);
        return c += b;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator-=(Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        auto a_data = a.data();
        auto b_data = b.data();
        for (auto i = 0u; i < M * N; ++i)
            a_data[i] -= b_data[i];
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
    Matrix<T, M, N> operator*(Matrix<T, M, N> a, T s)
    {
        for (unsigned i = 0; i < M; ++i)
        {
            for (unsigned j = 0; j < N; ++j)
                a[i][j] *= s;
        }
        return a;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator*=(Matrix<T, M, N>& a, T s)
    {
        for (auto& v : a)
            v *= s;
        return a;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N> operator*(T s, Matrix<T, M, N> a)
    {
        for (auto& v : a)
            v *= s;
        return a;
    }

    template <typename T, unsigned M, unsigned N>
    Matrix<T, M, N>& operator/=(Matrix<T, M, N>& a, T s)
    {
        for (auto& v : a)
            v /= s;
        return a;
    }

    template <typename T, typename U, unsigned M, unsigned N>
    Vector<decltype(T() * U()), M>
    operator*(const Matrix<T, M, N>& m, const Vector<U, N>& v)
    {
        using R = decltype(T() * U());
        Vector<R, M> result;
        for (auto i = 0u; i < M; ++i)
        {
            auto value = R();
            for (auto j = 0u; j < N; ++j)
                value += m[i][j] * v[j];
            result[i] = value;
        }
        return result;
    }

    template <typename T, typename U, unsigned M, unsigned N>
    Vector<decltype(T() * U()), M>
    operator*(const Vector<T, M>& v, const Matrix<U, M, N>& m)
    {
        using R = decltype(T() * U());
        Vector<R, N> result;
        for (auto i = 0u; i < N; ++i)
        {
            R value = 0;
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

    template <typename T, unsigned N>
    void transpose_inplace(Matrix<T, N, N>& m)
    {
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = i + 1; j < N; ++j)
                std::swap(m[i][j], m[j][i]);
        }
    }

    template <typename T, unsigned M, unsigned N, unsigned O>
    Matrix<T, M, O> multiply_transposed(const Matrix<T, M, N>& a,
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

    template <typename T, unsigned M, unsigned N,
        typename std::enable_if_t<std::is_integral_v<T>, int> = 0>
    bool are_equivalent(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b,
                        T = 0)
    {
        return a == b;
    }

    template <typename T, unsigned N,
              typename std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    bool are_equivalent(const Matrix<T, N, N>& a, const Matrix<T, N, N>& b,
                        double margin = 1e-12)
    {
        for (unsigned i = 0; i < N; ++i)
        {
            for (unsigned j = 0; j < N; ++j)
            {
                if (std::abs(a[i][j] - b[i][j]) > margin)
                    return false;
            }
        }
        return true;
    }

    using Matrix2I = Matrix<int, 2, 2>;
    using Matrix2F = Matrix<float, 2, 2>;
    using Matrix2D = Matrix<double, 2, 2>;

    using Matrix3I = Matrix<int, 3, 3>;
    using Matrix3F = Matrix<float, 3, 3>;
    using Matrix3D = Matrix<double, 3, 3>;

    using Matrix4I = Matrix<int, 4, 4>;
    using Matrix4F = Matrix<float, 4, 4>;
    using Matrix4D = Matrix<double, 4, 4>;
}
