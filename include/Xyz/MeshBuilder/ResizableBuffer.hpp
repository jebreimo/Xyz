//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace Xyz
{
    template <typename T>
    concept ResizableBuffer = requires(T t, size_t count)
    {
        typename T::value_type;
        t.resize(count);
        { t.size() } -> std::convertible_to<size_t>;
        { t.data() } -> std::convertible_to<void*>;
    }
    && std::is_standard_layout_v<typename T::value_type>
    && std::is_trivially_copyable_v<typename T::value_type>;
}
