//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstring>
#include <span>
#include <type_traits>

#include "ResizableBuffer.hpp"

namespace Xyz
{
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
                             size_t first_row = 0)
            : buffer_(buffer),
              rows_(first_row),
              stride_(stride),
              offset_(offset)
        {
            if (sizeof(ValueType) > (stride - offset) * sizeof(typename BufferType::value_type))
            {
                throw std::invalid_argument(
                    "ValueType is too large for the given stride and offset.");
            }
        }

        [[nodiscard]] size_t size() const
        {
            return rows_;
        }

        void reserve(size_t rows)
        {
            const size_t required_size = rows * stride_;
            if (static_cast<size_t>(buffer_.size()) < required_size)
                buffer_.resize(required_size);
        }

        [[nodiscard]] ValueType get(size_t row) const
        {
            ValueType value;
            auto ptr = reinterpret_cast<const char*>(buffer_.data());
            ptr += (row * stride_ + offset_)
                * sizeof(typename BufferType::value_type);
            memcpy(&value, ptr, sizeof(ValueType));
            return value;
        }

        void add(const ValueType& value)
        {
            reserve(rows_ + 1);
            set(rows_++, &value, sizeof(ValueType));
        }

        void add_n(const ValueType& value, size_t n)
        {
            reserve(rows_ + n);
            set_n(rows_, value, n);
            rows_ += n;
        }

    private:
        void set(size_t row, const void* bytes, size_t size)
        {
            auto ptr = reinterpret_cast<char*>(buffer_.data());
            ptr += (row * stride_ + offset_)
                * sizeof(typename BufferType::value_type);
            memcpy(ptr, bytes, size);
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

        BufferType& buffer_;
        size_t rows_;
        size_t stride_;
        size_t offset_;
    };
}
