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

        constexpr Vector() noexcept
            : m_Values()
        {}

        Vector(std::initializer_list<T> v)
        {
            if (v.size() != N)
                XYZ_THROW("Incorrect number of arguments.");
            auto it = v.begin();
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = *it++;
        }

        explicit Vector(T (& arr)[N]) noexcept
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = arr[i];
        }

        Vector(const Vector& other) noexcept
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = other[i];
        }

        Vector(const T* values, unsigned count)
        {
            if (count >= N)
            {
                std::copy(values, values + N, m_Values.begin());
            }
            else
            {
                std::copy(values, values + count, m_Values.begin());
                std::fill(m_Values.begin() + count, m_Values.end(), T());
            }
        }

        Vector& operator=(T (& arr)[N])
        {
            for (unsigned i = 0; i < N; ++i)
                m_Values[i] = arr[i];
            return *this;
        }

        Vector& operator=(const Vector<T, N>& other)
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
            return m_Values.data();
        }

        T* begin()
        {
            return m_Values.data();
        }

        const T* end() const
        {
            return m_Values.data() + N;
        }

        T* end()
        {
            return m_Values.data() + N;
        }

        T* data()
        {
            return m_Values;
        }

        const T* data() const
        {
            return m_Values.data();
        }

    private:
        std::array<T, N> m_Values;
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
    Vector<T, 2> makeVector2(T x, T y)
    {
        return Vector<T, 2>({x, y});
    }

    template<typename T>
    Vector<T, 3> makeVector3(T x, T y, T z)
    {
        return Vector<T, 3>({x, y, z});
    }

    template <typename T, typename U>
    Vector<T, 3> makeVector3(const Vector<U, 2>& v, T z)
    {
        return Vector<T, 3>({v[0], v[1], z});
    }

    template<typename T>
    Vector<T, 4> makeVector4(T x, T y, T z, T w)
    {
        return Vector<T, 4>({x, y, z, w});
    }

    template<typename T, typename U, unsigned N>
    Vector<T, N> vector_cast(const Vector<U, N>& v)
    {
        if constexpr (std::is_same<T, U>::value)
        {
            return v;
        }
        else
        {
            Vector<T, N> result;
            for (unsigned i = 0; i < N; ++i)
                result[i] = static_cast<T>(v[i]);
            return result;
        }
    }
}
