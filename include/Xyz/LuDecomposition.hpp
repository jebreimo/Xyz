//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-01-17.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <Xyz/Matrix.hpp>

namespace Xyz
{
    template <typename T, unsigned N>
    class LuDecomposition
    {
    public:
        explicit LuDecomposition(const Matrix<T, N, N>& matrix)
            : lu_(matrix),
              pivot_indices_{}
        {
            Vector<T, N> scaling_factors = calculate_scaling_factors();

            for (unsigned k = 0; k < N; ++k)
            {
                T max = T(0);
                unsigned imax = k;
                for (unsigned i = k; i < N; ++i)
                {
                    auto temp = scaling_factors[i] * std::abs(lu_[{i, k}]);
                    if (temp > max)
                    {
                        max = temp;
                        imax = i;
                    }
                }
                if (k != imax)
                {
                    swap_rows(lu_, k, imax);
                    determinant_ = -determinant_;
                    scaling_factors[imax] = scaling_factors[k];
                }
                pivot_indices_[k] = imax;
                if (lu_[{k, k}] == T(0))
                    lu_[{k, k}] = T(1e-20); // Prevent division by zero.
                for (unsigned i = k + 1; i < N; ++i)
                {
                    auto temp = lu_[{i, k}] /= lu_[{k, k}];
                    for (unsigned j = k + 1; j < N; ++j)
                    {
                        lu_[{i, j}] -= temp * lu_[{k, j}];
                    }
                }
            }
        }

        Vector<T, N> solve(const Vector<T, N>& b) const
        {
            Vector<T, N> x = b;
            unsigned ii = 0;
            for (unsigned i = 0; i < N; ++i)
            {
                auto ip = pivot_indices_[i];
                auto sum = x[ip];
                x[ip] = x[i];
                if (ii != 0)
                {
                    for (unsigned j = ii - 1; j < i; ++j)
                    {
                        sum -= lu_[{i, j}] * x[j];
                    }
                }
                else if (sum != T(0))
                {
                    ii = i + 1;
                }
                x[i] = sum;
            }

            for (unsigned i = N; i-- > 0;)
            {
                auto sum = x[i];
                for (unsigned j = i + 1; j < N; ++j)
                {
                    sum -= lu_[{i, j}] * x[j];
                }
                x[i] = sum / lu_[{i, i}];
            }
            return x;
        }

        template <unsigned M>
        [[nodiscard]] Matrix<T, N, M> solve(const Matrix<T, N, M>& b) const
        {
            Matrix<T, N, M> x;
            for (unsigned i = 0; i < M; ++i)
            {
                auto col = get_col(b, i);
                auto solved_col = solve(col);
                set_col(x, i, solved_col);
            }
            return x;
        }

        [[nodiscard]] T determinant() const
        {
            T det = determinant_;
            for (unsigned i = 0; i < N; ++i)
                det *= lu_[{i, i}];
            return det;
        }

        [[nodiscard]] Matrix<T, N, N> inverse() const
        {
            return solve(make_identity_matrix<T, N>());
        }

    private:
        Vector<T, N> calculate_scaling_factors()
        {
            Vector<T, N> scaling_factors;
            for (unsigned i = 0; i < N; ++i)
            {
                auto big = T(0);
                for (unsigned j = 0; j < N; ++j)
                {
                    auto temp = std::abs(lu_[{i, j}]);
                    if (temp > big)
                        big = temp;
                }
                if (big == T(0))
                    XYZ_THROW("Matrix is singular.");
                scaling_factors[i] = T(1) / big;
            }
            return scaling_factors;
        }

        Matrix<T, N, N> lu_;
        unsigned pivot_indices_[N];
        T determinant_ = T(1);
    };
}
