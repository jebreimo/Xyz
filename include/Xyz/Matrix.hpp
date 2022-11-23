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
    struct RowCol
    {
        RowCol() = default;
        RowCol(unsigned r, unsigned c) : row(r), col(c) {}
        unsigned row = 0;
        unsigned col = 0;
    };

    template <typename T, unsigned M, unsigned N>
    class Matrix
    {
    public:
        static constexpr size_t SIZE = M * N;
        static constexpr size_t ROWS = M;
        static constexpr size_t COLS = N;

        constexpr Matrix()
            : values()
        {}

        Matrix(std::initializer_list<T> v)
        {
            if (v.size() != SIZE)
                XYZ_THROW("Incorrect number of arguments.");
            std::copy(v.begin(), v.end(), std::begin(values));
        }

        explicit Matrix(T (&other)[SIZE])
        {
            std::copy(std::begin(other), std::end(other), std::begin(values));
        }

        Matrix(const Matrix& other)
        {
            std::copy(begin(other), end(other), std::begin(values));
        }

        Matrix& operator=(const Matrix& other)
        {
            if (&other == this)
                return *this;
            std::copy(begin(other), end(other), std::begin(values));
            return *this;
        }

        constexpr T& operator[](RowCol pos)
        {
            return values[pos.row * COLS + pos.col];
        }

        constexpr T operator[](RowCol pos) const
        {
            return values[pos.row * COLS + pos.col];
        }

        T values[SIZE];
    };

    template <typename T>
    class Matrix<T, 2, 2>
    {
    public:
        static constexpr size_t SIZE = 2 * 2;
        static constexpr size_t ROWS = 2;
        static constexpr size_t COLS = 2;

        constexpr Matrix()
            : values()
        {}

        Matrix(T a11, T a12, T a21, T a22)
            : values{a11, a12, a21, a22}
        {}

        explicit Matrix(T (&other)[SIZE])
        {
            std::copy(std::begin(other), std::end(other), std::begin(values));
        }

        Matrix(const Matrix& other)
        {
            std::copy(begin(other), end(other), std::begin(values));
        }

        Matrix& operator=(const Matrix& other)
        {
            if (&other == this)
                return *this;
            std::copy(begin(other), end(other), std::begin(values));
            return *this;
        }

        constexpr T& operator[](RowCol pos)
        {
            return values[pos.row * COLS + pos.col];
        }

        constexpr T operator[](RowCol pos) const
        {
            return values[pos.row * COLS + pos.col];
        }

        T values[SIZE];
    };

    template <typename T>
    class Matrix<T, 3, 3>
    {
    public:
        static constexpr size_t SIZE = 3 * 3;
        static constexpr size_t ROWS = 3;
        static constexpr size_t COLS = 3;

        constexpr Matrix()
            : values()
        {}

        Matrix(T a11, T a12, T a13,
               T a21, T a22, T a23,
               T a31, T a32, T a33)
            : values{a11, a12, a13,
                     a21, a22, a23,
                     a31, a32, a33}
        {}

        explicit Matrix(T (&other)[SIZE])
        {
            std::copy(std::begin(other), std::end(other), std::begin(values));
        }

        Matrix(const Matrix& other)
        {
            std::copy(begin(other), end(other), std::begin(values));
        }

        Matrix& operator=(const Matrix& other)
        {
            if (&other == this)
                return *this;
            std::copy(begin(other), end(other), std::begin(values));
            return *this;
        }

        constexpr T& operator[](RowCol pos)
        {
            return values[pos.row * COLS + pos.col];
        }

        constexpr T operator[](RowCol pos) const
        {
            return values[pos.row * COLS + pos.col];
        }

        T values[SIZE];
    };

    template <typename T>
    class Matrix<T, 4, 4>
    {
    public:
        static constexpr size_t SIZE = 4 * 4;
        static constexpr size_t ROWS = 4;
        static constexpr size_t COLS = 4;

        constexpr Matrix()
            : values()
        {}

        Matrix(T a11, T a12, T a13, T a14,
               T a21, T a22, T a23, T a24,
               T a31, T a32, T a33, T a34,
               T a41, T a42, T a43, T a44)
            : values{a11, a12, a13, a14,
                     a21, a22, a23, a24,
                     a31, a32, a33, a34,
                     a41, a42, a43, a44}
        {}

        explicit Matrix(T (&other)[SIZE])
        {
            std::copy(std::begin(other), std::end(other), std::begin(values));
        }

        Matrix(const Matrix& other)
        {
            std::copy(begin(other), end(other), std::begin(values));
        }

        Matrix& operator=(const Matrix& other)
        {
            if (&other == this)
                return *this;
            std::copy(begin(other), end(other), std::begin(values));
            return *this;
        }

        constexpr T& operator[](RowCol pos)
        {
            return values[pos.row * COLS + pos.col];
        }

        constexpr T operator[](RowCol pos) const
        {
            return values[pos.row * COLS + pos.col];
        }

        T values[SIZE];
    };

    template <typename T, unsigned M, unsigned N>
    constexpr T* begin(Matrix<T, M, N>& m)
    {
        return std::begin(m.values);
    }

    template <typename T, unsigned M, unsigned N>
    constexpr T* end(Matrix<T, M, N>& m)
    {
        return std::end(m.values);
    }

    template <typename T, unsigned M, unsigned N>
    constexpr const T* begin(const Matrix<T, M, N>& m)
    {
        return std::begin(m.values);
    }

    template <typename T, unsigned M, unsigned N>
    constexpr const T* end(const Matrix<T, M, N>& m)
    {
        return std::end(m.values);
    }

    template <typename T, unsigned M, unsigned N>
    Vector<T, N> row(const Matrix<T, M, N>& m, unsigned r)
    {
        return Vector<T, N>(&m.values[r * N], N);
    }

    template <typename T, unsigned M, unsigned N>
    void set_row(Matrix<T, M, N>& m, unsigned row, const Vector<T, N>& v)
    {
        std::copy(begin(v), end(v), m.values + row * N);
    }

    template <typename T, unsigned M, unsigned N>
    void set_row(Matrix<T, M, N>& m, unsigned row,
                 const T* values, unsigned count)
    {
        if (count != N)
            XYZ_THROW("Incorrect number of columns.");
        std::copy(values, values + count, m.values + row * N);
    }

    template <typename T, unsigned M, unsigned N>
    Vector<T, M> col(const Matrix<T, M, N>& m, unsigned c)
    {
        Vector<T, M> result;
        auto ptr = m.values + c;
        for (unsigned i = 0; i < M; ++i)
        {
            result[i] = *ptr;
            ptr += N;
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    void set_col(Matrix<T, M, N>& m, unsigned c, const Vector<T, M>& v)
    {
        auto ptr = m.values + c;
        for (unsigned i = 0; i < M; ++i)
        {
            *ptr = v[i];
            ptr += N;
        }
    }

    template <typename T, unsigned M, unsigned N>
    void set_col(Matrix<T, M, N>& m, unsigned c,
                 const T* values, unsigned count)
    {
        if (count != M)
            XYZ_THROW("Incorrect number of columns.");
        auto ptr = m.values + c;
        for (unsigned i = 0; i < M; ++i)
        {
            *ptr = values[i];
            ptr += N;
        }
    }

    template <typename T, unsigned N>
    Matrix<T, N, N> make_identity_matrix()
    {
        static Matrix<T, N, N> matrix;
        if (matrix[{0, 0}] == 0)
        {
            for (unsigned i = 0; i < N; ++i)
                matrix[{i, i}] = 1;
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
                result[{i, j}] = m[{i_m, j_m}];
            }
        }
        return result;
    }

    template <typename T, unsigned N, unsigned M>
    bool operator==(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b)
    {
        for (auto i = 0u; i < M * N; ++i)
        {
            if (a.values[i] != b.values[i])
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
        auto a_data = a.values;
        auto b_data = b.values;
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
        auto a_data = a.values;
        auto b_data = b.values;
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
                    v += a[{i, k}] * b[{k, j}];
                result[{i, j}] = v;
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
                a[{i, j}] *= s;
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
                value += m[{i, j}] * v[j];
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
                value += v[j] * m[{j, i}];
            result[i] = value;
        }
        return result;
    }

    template <typename T, unsigned M, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& m)
    {
        os << m[{0, 0}];
        for (auto j = 1u; j < N; ++j)
            os << " " << m[{0, j}];
        for (auto i = 1u; i < M; ++i)
        {
            os << " |";
            for (auto j = 0u; j < N; ++j)
                os << " " << m[{i, j}];
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
                result[{i, j}] = m[{j, i}];
        }
        return result;
    }

    template <typename T, unsigned N>
    void transpose_inplace(Matrix<T, N, N>& m)
    {
        for (auto i = 0u; i < N; ++i)
        {
            for (auto j = i + 1; j < N; ++j)
                std::swap(m[{i, j}], m[{j, i}]);
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
                    v += a[{i, k}] * b[{j, k}];
                result[{i, j}] = v;
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
                if (std::abs(a[{i, j}] - b[{i, j}]) > margin)
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
