//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Matrix.hpp"

namespace Xyz
{
    namespace affine
    {
        /**
         * @brief Converts a vector to homogeneous coordinates by adding a
         *  coordinate with value 1 at the end.
         */
        template <std::floating_point T, unsigned N>
        [[nodiscard]]
        Vector<T, N + 1> to_hg(const Vector<T, N>& v)
        {
            return push_back(v, 1);
        }

        /**
         * @brief Converts a vector from homogeneous coordinates by removing the
         *  last coordinate if it is 1, or dividing all coordinates by the last
         *  coordinate if it is not 1.
         * @throws XyzException if the last coordinate is zero.
         */
        template <std::floating_point T, unsigned N>
        [[nodiscard]]
        Vector<T, N - 1> from_hg(const Vector<T, N>& v)
        {
            if (v[N - 1] == 1)
                return drop_back(v);

            if (v[N - 1] == 0)
                XYZ_THROW("Cannot convert from homogeneous coordinates: last coordinate is zero.");

            return drop_back(v) / v[N - 1];
        }

        template <typename T>
        Matrix<T, 3, 3> rotate2(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                c, -s, 0,
                s, c, 0,
                0, 0, 1
            };
        }

        /**
         * @brief Rotation around the z axis.
         */
        template <typename T>
        Matrix<T, 4, 4> rotate_z(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> rotate_y(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> rotate_x(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> rotate3(T angle, const Vector<T, 3>& axis)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            auto cx = axis * ((1 - c) * axis[0]);
            auto cy = axis * ((1 - c) * axis[1]);
            auto cz = axis * ((1 - c) * axis[2]);
            auto sa = s * axis;
            return {
                cx[0] + c, cx[1] - sa[2], cx[2] + sa[1], 0,
                cy[0] + sa[2], cy[1] + c, cy[2] - sa[0], 0,
                cz[0] - sa[1], cz[1] + sa[0], cz[2] + c, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 3, 3> transposed_rotate2_hg(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return Matrix<T, 3, 3>{
                c, s, 0,
                -s, c, 0,
                0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_rotate_z_hg(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return Matrix<T, 4, 4>{
                c, s, 0, 0,
                -s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_rotate_y_hg(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return Matrix<T, 4, 4>{
                c, 0, -s, 0,
                0, 1, 0, 0,
                s, 0, c, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_rotate_x_hg(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return Matrix<T, 4, 4>{
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_rotate3_hg(T angle, const Vector<T, 3>& axis)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            auto cx = axis * ((1 - c) * axis[0]);
            auto cy = axis * ((1 - c) * axis[1]);
            auto cz = axis * ((1 - c) * axis[2]);
            auto sa = s * axis;
            return Matrix<T, 4, 4>{
                cx[0] + c, cy[0] + sa[2], cz[0] - sa[1], 0,
                cx[1] - sa[2], cy[1] + c, cz[1] + sa[0], 0,
                cx[2] + sa[1], cy[2] - sa[0], cz[2] + c, 0,
                0, 0, 0, 1
            };
        }
    }

    namespace linear
    {
        template <typename T>
        Matrix<T, 2, 2> rotate2(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                c, -s,
                s, c,
            };
        }

        template <typename T>
        Matrix<T, 3, 3> rotate_z(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                c, -s, 0,
                s, c, 0,
                0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 3, 3> rotate_x(T angle)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            return {
                1, 0, 0,
                0, c, -s,
                0, s, c
            };
        }

        template <typename T>
        Matrix<T, 3, 3> rotate3(T angle, const Vector<T, 3>& axis)
        {
            auto c = std::cos(angle);
            auto s = std::sin(angle);
            auto cx = axis * ((1 - c) * axis[0]);
            auto cy = axis * ((1 - c) * axis[1]);
            auto cz = axis * ((1 - c) * axis[2]);
            auto sa = s * axis;
            return {
                cx[0] + c, cx[1] - sa[2], cx[2] + sa[1],
                cy[0] + sa[2], cy[1] + c, cy[2] - sa[0],
                cz[0] - sa[1], cz[1] + sa[0], cz[2] + c
            };
        }
    }
}
