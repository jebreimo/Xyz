//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <algorithm>
#include <complex>
#include <cstdlib>
#include <limits>
#include <type_traits>

namespace Xyz
{
    template <typename T,
              std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    struct ComplexApprox
    {
        constexpr static T DEFAULT_MARGIN = 100 * std::numeric_limits<T>::epsilon();

        constexpr explicit ComplexApprox(
            std::complex<T> value,
            std::complex<T> margin = {DEFAULT_MARGIN, DEFAULT_MARGIN}
            ) noexcept
            : value(value),
              margin(margin)
        {}

        std::complex<T> value;
        std::complex<T> margin;
    };

    namespace Details
    {
        template <typename T, typename U,
                  std::enable_if<std::is_same_v<T, decltype(T() + U())>, int> = 0>
        ComplexApprox<T>
        merge_margins(const ComplexApprox<T>& a, const ComplexApprox<U>& b)
        {
            std::complex<T> margin = {
                std::max(a.margin.real(), b.margin.real()),
                std::max(a.margin.imag(), b.margin.imag())
            };
            return ComplexApprox<T>(a.value, margin);
        }

        template <typename T, typename U,
                  std::enable_if<!std::is_same_v<T, decltype(T() + U())>, int> = 0>
        auto merge_margins(const ComplexApprox<T>& a, const ComplexApprox<U>& b)
            -> ComplexApprox<decltype(T() + U())>
        {
            using V = decltype(T() + U());
            std::complex<V> margin = {
                std::max<V>(a.margin.real(), b.margin.real()),
                std::max<V>(a.margin.imag(), b.margin.imag())
            };
            return ComplexApprox<V>(
                std::complex<V>(a.value.real(), a.value.imag()),
                margin
                );
        }
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator==(std::complex<U> v, const ComplexApprox<T>& m)
    {
        return v.real() + m.margin.real() >= m.value.real()
               && m.value.real() + m.margin.real() >= v.real()
               && v.imag() + m.margin.imag() >= m.value.imag()
               && m.value.imag() + m.margin.imag() >= v.imag();
    }

    template <typename T, typename U,
              std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator==(const ComplexApprox<T>& m, std::complex<U> v)
    {
        return v.real() + m.margin.real() >= m.value.real()
               && m.value.real() + m.margin.real() >= v.real()
               && v.imag() + m.margin.imag() >= m.value.imag()
               && m.value.imag() + m.margin.imag() >= v.imag();
    }

    template <typename T, typename U>
    constexpr bool operator==(const ComplexApprox<T>& a, const ComplexApprox<U>& b)
    {
        return Details::merge_margins(a, b) == b.value;
    }

    template <typename T, typename U,
        std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator!=(std::complex<U> v, const ComplexApprox<T>& m)
    {
        return !(v == m);
    }

    template <typename T, typename U,
        std::enable_if_t<std::is_arithmetic_v<U>, int> = 0>
    constexpr bool operator!=(const ComplexApprox<T>& m, std::complex<U> v)
    {
        return !(m == v);
    }

    template <typename T, typename U>
    constexpr bool operator!=(const ComplexApprox<T>& a, const ComplexApprox<U>& b)
    {
        return !(a == b);
    }
}
