//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Matrix.hpp"

namespace Xyz
{
    template <typename T>
    Matrix<T, 3, 3> scale3(T sx, T sy)
    {
        return Matrix<T, 3, 3>{
                sx, 0, 0,
                0, sy, 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3, 3> scale3(Vector<T, 2>& scales)
    {
        return scale3(scales[0], scales[1]);
    }

    template <typename T>
    Matrix<T, 4, 4> scale4(T sx, T sy, T sz)
    {
        return Matrix<T, 4, 4>{
                sx, 0, 0, 0,
                0, sy, 0, 0,
                0, 0, sz, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> scale4(Vector<T, 3>& scales)
    {
        return scale4(scales[0], scales[1], scales[2]);
    }

    template <typename T>
    Matrix<T, 3, 3> rotate3(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 3, 3>{
                c, -s, 0,
                s, c, 0,
                0, 0, 1};
    }

    /**
     * @brief Rotation around the z axis.
     */
    template <typename T>
    Matrix<T, 4, 4> rotate_z(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> rotate_y(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1};
    }

    template <unsigned N, typename T>
    Matrix<T, 4, 4> rotate_x(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3, 3> translate3(T x, T y)
    {
        return Matrix<T, 3, 3>{
                1, 0, x,
                0, 1, y,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3, 3> translate3(const Vector<T, 2>& offsets)
    {
        return translate3(offsets[0], offsets[1]);
    }

    template <typename T>
    Matrix<T, 4, 4> rotate4(T angle, const Vector<T, 3>& axis)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        auto cx = axis * ((1 - c) * axis[0]);
        auto cy = axis * ((1 - c) * axis[1]);
        auto cz = axis * ((1 - c) * axis[2]);
        auto sa = s * axis;
        return Matrix<T, 4, 4>{
                cx[0] + c, cx[1] - sa[2], cx[2] + sa[1], 0,
                cy[0] + sa[2], cy[1] + c, cy[2] - sa[0], 0,
                cz[0] - sa[1], cz[1] + sa[0], cz[2] + c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> translate4(T x, T y, T z)
    {
        return Matrix<T, 4, 4>{
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> translate4(const Vector<T, 3>& offsets)
    {
        return translate4(offsets[0], offsets[1], offsets[2]);
    }

    template <typename T>
    Matrix<T, 3, 3> transposed_rotate3(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 3, 3>{
                c, s, 0,
                -s, c, 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3, 3> transposed_translate3(T x, T y)
    {
        return Matrix<T, 3, 3>{
                1, 0, 0,
                0, 1, 0,
                x, y, 1};
    }

    template <typename T>
    Matrix<T, 3, 3> transposed_translate3(const Vector<T, 2>& offsets)
    {
        return transpose_translate(offsets[0], offsets[1]);
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_rotate_z(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                c, s, 0, 0,
                -s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_rotate_y(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                c, 0, -s, 0,
                0, 1, 0, 0,
                s, 0, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_rotate_x(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4, 4>{
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_rotate4(T angle, const Vector<T, 3>& axis)
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
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_translate4(T x, T y, T z)
    {
        return Matrix<T, 4, 4>{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1};
    }

    template <typename T>
    Matrix<T, 4, 4> transposed_translate4(const Vector<T, 3>& offsets)
    {
        return transposed_translate4(offsets[0], offsets[1], offsets[2]);
    }
}
