//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include "MatrixClass.hpp"
#include "VectorClass.hpp"

namespace Xyz { namespace Transforms {

    template <unsigned N, typename T>
    Matrix<T, N> scale(T s)
    {
        Matrix<T, N> m;
        for (auto i = 0u; i < N - 1; ++i)
            m[i][i] = s;
        m[N - 1][N - 1] = 1;
        return m;
    }

    template <unsigned N, typename T>
    Matrix<T, N + 1> scale(Vector<T, N>& scales)
    {
        Matrix<T, N + 1> m;
        for (auto i = 0u; i < N; ++i)
            m[i][i] = scales[i];
        m[N][N] = 1;
        return m;
    }

    template <unsigned N, typename T>
    Matrix<T, N> rotateZ(T angle)
    {
        static_assert(N > 1, "Matrix must be of dimension 2 or greater.");
        Matrix<T, N> m;
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        m[0][0] = c;
        m[0][1] = -s;
        m[1][0] = s;
        m[1][1] = c;
        for (auto i = 2u; i < N; ++i)
            m[i][i] = 1;
        return m;
    }

    template <unsigned N, typename T>
    Matrix<T, N> rotateY(T angle)
    {
        static_assert(N > 2, "Matrix must be of dimension 3 or greater.");
        Matrix<T, N> m;
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        m[0][0] = c;
        m[0][2] = s;
        m[1][1] = 1;
        m[2][0] = -s;
        m[2][2] = c;
        for (auto i = 3u; i < N; ++i)
            m[i][i] = 1;
        return m;
    }

    template <unsigned N, typename T>
    Matrix<T, 4> rotateX(T angle)
    {
        static_assert(N > 2, "Matrix must be of dimension 3 or greater.");
        Matrix<T, N> m;
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        m[0][0] = 1;
        m[1][1] = c;
        m[1][2] = -s;
        m[2][1] = s;
        m[2][2] = c;
        for (auto i = 3u; i < N; ++i)
            m[i][i] = 1;
        return m;
    }
}}

namespace Xyz { namespace HomogeneousTransforms {

    template <typename T>
    Matrix<T, 3> scale(T s)
    {
        return Matrix<T, 3>{
                s, 0, 0,
                0, s, 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3> scale3(const Vector<T, 2>& s)
    {
        return Matrix<T, 3>{
                s[0], 0, 0,
                0, s[1], 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3> rotate3(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 3>{
                c, -s, 0,
                s, c, 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3> translate3(const Vector<T, 2>& offsets)
    {
        return Matrix<T, 4>{
                1, 0, offsets[0],
                0, 1, offsets[1],
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> scale4(T s)
    {
        return Matrix<T, 4>{
                s, 0, 0, 0,
                0, s, 0, 0,
                0, 0, s, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> scale4(const Vector<T, 3>& s)
    {
        return Matrix<T, 4>{
                s[0], 0, 0, 0,
                0, s[1], 0, 0,
                0, 0, s[2], 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotateZ4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotateY4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotateX4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotate4(T angle, const Vector<T, 3>& axis)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        auto cx = axis * ((1 - c) * axis[0]);
        auto cy = axis * ((1 - c) * axis[1]);
        auto cz = axis * ((1 - c) * axis[2]);
        auto sa = s * axis;
        return Matrix<T, 4>{
                cx[0] + c, cx[1] - sa[2], cx[2] + sa[1], 0,
                cy[0] + sa[2], cy[1] + c, cy[2] - sa[0], 0,
                cz[0] - sa[1], cz[1] + sa[0], cz[2] + c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> translate4(const Vector<T, 3>& offsets)
    {
        return Matrix<T, 4>{
                1, 0, 0, offsets[0],
                0, 1, 0, offsets[1],
                0, 0, 1, offsets[2],
                0, 0, 0, 1};
    }

}}

namespace Xyz { namespace TransposedHomogeneousTransforms {

    template <typename T>
    Matrix<T, 3> rotate3(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 3>{
                c, s, 0,
                -s, c, 0,
                0, 0, 1};
    }

    template <typename T>
    Matrix<T, 3> translate3(const Vector<T, 2>& offsets)
    {
        return Matrix<T, 3>{
                1, 0, 0,
                0, 1, 0,
                offsets[0], offsets[1], 1};
    }

    template <typename T>
    Matrix <T, 4> rotateZ4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                c, s, 0, 0,
                -s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotateY4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                c, 0, -s, 0,
                0, 1, 0, 0,
                s, 0, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotateX4(T angle)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        return Matrix<T, 4>{
                1, 0, 0, 0,
                0, c, s, 0,
                0, -s, c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> rotate4(T angle, const Vector<T, 3>& axis)
    {
        auto c = std::cos(angle);
        auto s = std::sin(angle);
        auto cx = axis * ((1 - c) * axis[0]);
        auto cy = axis * ((1 - c) * axis[1]);
        auto cz = axis * ((1 - c) * axis[2]);
        auto sa = s * axis;
        return Matrix<T, 4>{
                cx[0] + c, cy[0] + sa[2], cz[0] - sa[1], 0,
                cx[1] - sa[2], cy[1] + c, cz[1] + sa[0], 0,
                cx[2] + sa[1], cy[2] - sa[0], cz[2] + c, 0,
                0, 0, 0, 1};
    }

    template <typename T>
    Matrix<T, 4> translate4(const Vector<T, 3>& offsets)
    {
        return Matrix<T, 4>{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                offsets[0], offsets[1], offsets[2], 1};
    }

}}
