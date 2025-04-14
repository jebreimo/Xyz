//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cmath>
#include <complex>
#include <optional>
#include <type_traits>

namespace Xyz
{
    template <typename T, typename RESULT = decltype(sqrt(T()))>
    constexpr std::optional<std::pair<RESULT, RESULT>>
    solve_real_quadratic_equation(T a, T b, T c)
    {
        auto bb = b * b;
        auto ac4 = a * c * 4;
        if (bb < ac4)
            return {};
        auto root = sqrt(bb - ac4);
        return std::pair{(-b - root) / (2 * a), (-b + root) / (2 * a)};
    }

    template <typename T,
              std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
    constexpr std::pair<std::complex<T>, std::complex<T>>
    solve_complex_quadratic_equation(std::complex<T> a,
                                     std::complex<T> b,
                                     std::complex<T> c)
    {
        auto root = sqrt(b * b - T(4) * a * c);
        return std::pair{(-b - root) / (T(2) * a), (-b + root) / (T(2) * a)};
    }
}
