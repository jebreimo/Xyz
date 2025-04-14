//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-08.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <array>
#include "Matrix.hpp"

namespace Xyz
{
    namespace Details
    {
        template <typename T>
        Matrix<T, 3, 3> get_transposed_cofactors(const Matrix<T, 3, 3>& m)
        {
            return Matrix<T, 3, 3>{
                    m[{1, 1}] * m[{2, 2}] - m[{1, 2}] * m[{2, 1}],
                    -(m[{0, 1}] * m[{2, 2}] - m[{0, 2}] * m[{2, 1}]),
                    m[{0, 1}] * m[{1, 2}] - m[{0, 2}] * m[{1, 1}],
                    -(m[{1, 0}] * m[{2, 2}] - m[{1, 2}] * m[{2, 0}]),
                    m[{0, 0}] * m[{2, 2}] - m[{0, 2}] * m[{2, 0}],
                    -(m[{0, 0}] * m[{1, 2}] - m[{0, 2}] * m[{1, 0}]),
                    m[{1, 0}] * m[{2, 1}] - m[{1, 1}] * m[{2, 0}],
                    -(m[{0, 0}] * m[{2, 1}] - m[{0, 1}] * m[{2, 0}]),
                    m[{0, 0}] * m[{1, 1}] - m[{0, 1}] * m[{1, 0}]
            };
        }

        template <typename T>
        T get_cofactor(const Matrix<T, 4, 4>& m, const std::array<T, 7>& a,
                      unsigned row, const std::array<unsigned, 3>& columns)
        {
            return m[{row, columns[0]}] * a[columns[1] * 2 + columns[2] - 1]
                   - m[{row, columns[1]}] * a[columns[0] * 2 + columns[2] - 1]
                   + m[{row, columns[2]}] * a[columns[0] * 2 + columns[1] - 1];
        }

        template <typename T>
        Matrix<T, 4, 4> get_transposed_cofactors(const Matrix<T, 4, 4>& m)
        {
            Matrix<T, 4, 4> c;
            std::array<T, 7> a = {};
            for (unsigned k = 0; k < 4; k += 2)
            {
                // Calculate the six 2x2 determinants that are used to
                // calculate the 3x3 determinants. A naive recursive
                // solution would calculate these determinants 24 times.
                for (unsigned i = 0; i < 3; ++i)
                {
                    for (unsigned j = i + 1; j < 4; ++j)
                    {
                        auto p0 = m[{k, i}] * m[{k + 1, j}];
                        auto p1 = m[{k, j}] * m[{k + 1, i}];
                        a[i * 2 + j - 1] = p0 - p1;
                    }
                }
                int sign = 1;
                for (unsigned i = 2 - k; i < 4 - k; ++i)
                {
                    std::array<unsigned, 3> columns = {1, 2, 3};
                    auto other_row = 5 - 2 * k - i;
                    for (unsigned j = 0; j < 4; ++j)
                    {
                        c[{j, i}] = sign * get_cofactor(m, a, other_row, columns);
                        sign = -sign;
                        if (j < 3)
                            columns[j] = j;
                    }
                    sign = -sign;
                }
            }
            return c;
        }
    }

    template <typename T, unsigned N,
              std::enable_if_t<N == 3 || N == 4, int> = 0>
    Matrix<T, N, N> invert(const Matrix<T, N, N>& m)
    {
        auto c = Details::get_transposed_cofactors(m);
        T det = 0;
        for (unsigned i = 0; i < N; ++i)
            det += m[{0, i}] * c[{i, 0}];
        if (det == 0)
            XYZ_THROW("The matrix is not invertible.");
        c *= T(1) / det;
        return c;
    }

    template <typename T>
    Matrix<T, 1, 1> invert(const Matrix<T, 1, 1>& m)
    {
        if (m[{0, 0}] == 0)
            XYZ_THROW("The matrix is not invertible.");
        return Matrix<typename FloatType<T>::type, 1, 1>{
            1.0 / m[{0, 0}]};
    }

    template <typename T>
    Matrix<T, 2, 2> invert(const Matrix<T, 2, 2>& m)
    {
        using Float = typename FloatType<T>::type;
        auto det = m[{0, 0}] * m[{1, 1}] - m[{0, 1}] * m[{1, 0}];
        if (det == 0)
            XYZ_THROW("The matrix is not invertible.");
        auto w = Float(1) / det;
        return Matrix<Float, 2, 2>{m[{1, 1}] * w, -m[{0, 1}] * w,
                                   -m[{1, 0}] * w, m[{0, 0}] * w};
    }
}
