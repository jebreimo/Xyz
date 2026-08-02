//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <span>
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

    template <typename T>
    concept AssignableType = std::is_standard_layout_v<T>
        && std::is_trivially_copyable_v<T>;

    template <AssignableType ValueType, ResizableBuffer BufferType>
    class MeshAttributeBuilder
    {
    public:
        MeshAttributeBuilder(BufferType& buffer,
                             size_t stride,
                             size_t offset = 0,
                             size_t rows = 0)
            : buffer_(buffer),
              rows_(rows),
              stride_(stride),
              offset_(offset)
        {
            if (sizeof(ValueType) > (stride - offset) * sizeof(typename BufferType::value_type))
            {
                throw std::invalid_argument(
                    "ValueType is too large for the given stride and offset.");
            }
        }

        void resize(size_t rows)
        {
            const size_t required_size = rows * stride_;
            if (static_cast<size_t>(buffer_.size()) < required_size)
                buffer_.resize(required_size);
            rows_ = rows;
        }

        void add(const ValueType& value)
        {
            resize(rows_ + 1);
            set(rows_ - 1, value);
        }

        void add_n(const ValueType& value, size_t n)
        {
            resize(rows_ + n);
            set_n(rows_ - n, value, n);
        }

        ValueType get(size_t row) const
        {
            ValueType value;
            auto ptr = reinterpret_cast<const char*>(buffer_.data());
            ptr += (row * stride_ + offset_)
                * sizeof(typename BufferType::value_type);
            memcpy(&value, ptr, sizeof(ValueType));
            return value;
        }

        void set(size_t row, const ValueType& value)
        {
            set(row, &value, sizeof(ValueType));
        }

        void set_n(size_t first_row, const ValueType& value, size_t n)
        {
            auto ptr = reinterpret_cast<char*>(buffer_.data());
            ptr += (first_row * stride_ + offset_)
                * sizeof(typename BufferType::value_type);
            for (size_t i = 0; i < n; ++i)
            {
                memcpy(ptr, &value, sizeof(ValueType));
                ptr += stride_ * sizeof(typename BufferType::value_type);
            }
        }

    private:
        void set(size_t row, const void* bytes, size_t size)
        {
            assert(size < (stride_ - offset_) * sizeof(typename BufferType::value_type));
            auto ptr = reinterpret_cast<char*>(buffer_.data());
            ptr += (row * stride_ + offset_)
                * sizeof(typename BufferType::value_type);
            memcpy(ptr, bytes, size);
        }

        BufferType& buffer_;
        size_t rows_;
        size_t stride_;
        size_t offset_;
    };
}
