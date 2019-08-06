//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <ostream>
#include <type_traits>
#include "VectorClass.hpp"

namespace Xyz
{
#define XYZ_TYPE_OF(expr) \
    typename std::remove_cv<decltype(expr)>::type

    template <typename T, unsigned N>
    bool operator==(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
        {
            if (u[i] != v[i])
                return false;
        }
        return true;
    }

    template <typename T, unsigned N>
    bool operator!=(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        return !(u == v);
    }

    template <typename T, unsigned N>
    Vector<T, N>& operator+=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] += v[i];
        return u;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N>& operator+=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] += scalar;
        return u;
    }

    template <typename T, unsigned N>
    Vector<T, N>& operator-=(Vector<T, N>& u, const Vector<T, N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] -= v[i];
        return u;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N>& operator-=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] -= scalar;
        return u;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N>& operator*=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] *= scalar;
        return u;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N>& operator/=(Vector<T, N>& u, S scalar)
    {
        for (unsigned i = 0; i < N; ++i)
            u[i] /= scalar;
        return u;
    }

    template <typename T, unsigned N>
    Vector<T, N> operator-(Vector<T, N>&& v)
    {
        for (unsigned i = 0; i < N; ++i)
            v[i] = -v[i];
        return v;
    }

    template <typename T, unsigned N>
    Vector<T, N> operator-(const Vector<T, N>& v)
    {
        return -Vector<T, N>(v);
    }

    template <typename T, unsigned N>
    Vector<T, N> operator+(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] + v[i];
        return w;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N> operator+(const Vector<T, N>& u, S scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] + scalar;
        return w;
    }

    template <typename T, unsigned N>
    Vector<T, N> operator-(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] - v[i];
        return w;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N> operator-(const Vector<T, N>& u, S scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] - scalar;
        return w;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N> operator/(const Vector<T, N>& u, S scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] / scalar;
        return w;
    }

    template <typename T, unsigned N>
    T operator*(const Vector<T, N>& u, const Vector<T, N>& v)
    {
        T result = 0;
        for (unsigned i = 0; i < N; ++i)
            result += u[i] * v[i];
        return result;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N> operator*(const Vector<T, N>& u, S scalar)
    {
        Vector<T, N> w;
        for (unsigned i = 0; i < N; ++i)
            w[i] = u[i] * scalar;
        return w;
    }

    template <typename T, typename S, unsigned N>
    Vector<T, N> operator*(S scalar, const Vector<T, N>& v)
    {
        return v * scalar;
    }

    template <typename T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
    {
        const T* it = v.begin();
        os << "[" << *it;
        while (++it != v.end())
            os << ", " << *it;
        return os << "]";
    }
}
