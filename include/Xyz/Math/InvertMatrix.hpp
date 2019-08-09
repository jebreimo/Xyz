//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-08.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <Xyz/Utilities/XyzException.hpp>

namespace Xyz
{
    namespace Details
    {
        template <typename T>
        Matrix<T, 3, 3> getTransposedCofactors(const Matrix<T, 3, 3>& m)
        {
            return Matrix<T, 3, 3>{
                    m[1][1] * m[2][2] - m[1][2] * m[2][1],
                    -(m[0][1] * m[2][2] - m[0][2] * m[2][1]),
                    m[0][1] * m[1][2] - m[0][2] * m[1][1],
                    -(m[1][0] * m[2][2] - m[1][2] * m[2][0]),
                    m[0][0] * m[2][2] - m[0][2] * m[2][0],
                    -(m[0][0] * m[1][2] - m[0][2] * m[1][0]),
                    m[1][0] * m[2][1] - m[1][1] * m[2][0],
                    -(m[0][0] * m[2][1] - m[0][1] * m[2][0]),
                    m[0][0] * m[1][1] - m[0][1] * m[1][0]
            };
        }

        template <typename T>
        T getCofactor(const Matrix<T, 4, 4>& m, const std::array<T, 7>& a,
                      unsigned row, const std::array<unsigned, 3>& columns)
        {
            return m[row][columns[0]] * a[columns[1] * 2 + columns[2] - 1]
                   - m[row][columns[1]] * a[columns[0] * 2 + columns[2] - 1]
                   + m[row][columns[2]] * a[columns[0] * 2 + columns[1] - 1];
        }

        template <typename T>
        Matrix<T, 4, 4> getTransposedCofactors(const Matrix<T, 4, 4>& m)
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
                        auto p0 = m[k][i] * m[k + 1][j];
                        auto p1 = m[k][j] * m[k + 1][i];
                        a[i * 2 + j - 1] = p0 - p1;
                    }
                }
                int sign = 1;
                for (unsigned i = 2 - k; i < 4 - k; ++i)
                {
                    std::array<unsigned, 3> columns = {1, 2, 3};
                    auto otherRow = 5 - 2 * k - i;
                    for (unsigned j = 0; j < 4; ++j)
                    {
                        c[j][i] = sign * getCofactor(m, a, otherRow, columns);
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

    template <typename T, unsigned N>
    Matrix<double, N, N> invert(const Matrix<T, N, N>& m)
    {
        auto c = Details::getTransposedCofactors(m);
        double det = 0;
        for (unsigned i = 0; i < N; ++i)
            det += m[0][i] * c[i][0];
        if (det == 0)
            XYZ_THROW("The matrix is not invertible.");
        c *= 1.0 / det;
        return c;
    }

    template <typename T>
    Matrix<double, 2, 2> invert(const Matrix<T, 2, 2>& m)
    {
        auto det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        if (det == 0)
            XYZ_THROW("The matrix is not invertible.");
        auto w = 1.0 / det;
        return Matrix<double, 2, 2>{
                m[1][1] * w, -m[0][1] * w,
                -m[1][0] * w, m[0][0] * w};
    }
}
