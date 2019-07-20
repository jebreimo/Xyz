//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <array>
#include <initializer_list>
#include "Xyz/Utilities/XyzException.hpp"

namespace Xyz
{
    template<typename T, unsigned N>
    class Vector
    {
    public:
        typedef T ValueType;

        static constexpr unsigned size()
        {
            return N;
        }

        Vector() = default;

        Vector(std::initializer_list<T> v)
        {
            if (v.size() != N)
                XYZ_THROW("Incorrect number of arguments.");
            auto it = v.begin();
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = *it++;
        }

        template<typename U>
        Vector(U (& arr)[N])
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = arr[i];
        }

        template<typename U>
        Vector(const Vector<U, N>& other)
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = other[i];
        }

        template<typename U>
        Vector& operator=(U (& arr)[N])
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = arr[i];
            return *this;
        }

        template<typename U>
        Vector& operator=(const Vector<U, N>& other)
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = other[i];
            return *this;
        }

        explicit operator bool() const
        {
            for (unsigned i = 0; i < N; ++i)
            {
                if (m_Values[i])
                    return true;
            }
            return false;
        }

        const T& operator[](unsigned i) const
        {
            return m_Values[i];
        }

        T& operator[](unsigned i)
        {
            return m_Values[i];
        }

        const T* begin() const
        {
            return m_Values;
        }

        T* begin()
        {
            return m_Values;
        }

        const T* end() const
        {
            return &m_Values[N];
        }

        T* end()
        {
            return &m_Values[N];
        }

        T* data()
        {
            return m_Values;
        }

        const T* data() const
        {
            return m_Values;
        }

    private:
        T m_Values[N];
    };

    template<typename T, unsigned N>
    const T* begin(const Vector<T, N>& v)
    {
        return v.begin();
    }

    template<typename T, unsigned N>
    const T* end(const Vector<T, N>& v)
    {
        return v.end();
    }

    template<typename T, unsigned N>
    T* begin(Vector<T, N>& v)
    {
        return v.begin();
    }

    template<typename T, unsigned N>
    T* end(Vector<T, N>& v)
    {
        return v.end();
    }

    template<typename T>
    Vector<T, 2> makeVector(T x, T y)
    {
        return Vector<T, 2>({x, y});
    }

    template<typename T>
    Vector<T, 3> makeVector(T x, T y, T z)
    {
        return Vector<T, 3>({x, y, z});
    }

    template<typename T>
    Vector<T, 4> makeVector(T x, T y, T z, T w)
    {
        return Vector<T, 4>({x, y, z, w});
    }

    template<typename T, typename U, unsigned N>
    Vector<T, N> makeVector(const Vector<U, N>& v)
    {
        return Vector<T, N>(v);
    }

}
