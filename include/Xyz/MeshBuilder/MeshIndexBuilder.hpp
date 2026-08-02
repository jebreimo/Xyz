//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-02.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include <vector>

namespace Xyz
{
    template <std::integral T>
    class MeshIndexBuilder
    {
    public:
        MeshIndexBuilder(std::vector<T>& buffer, T base_index = 0)
            : buffer_(buffer),
              base_index_(base_index)
        {}

        void reserve(size_t size)
        {
            buffer_.reserve(size);
        }

        void add(T index)
        {
            buffer_.push_back(base_index_ + index);
        }

        void add(T index0, T index1)
        {
            buffer_.push_back(base_index_ + index0);
            buffer_.push_back(base_index_ + index1);
        }

        void add(T index0, T index1, T index2)
        {
            buffer_.push_back(base_index_ + index0);
            buffer_.push_back(base_index_ + index1);
            buffer_.push_back(base_index_ + index2);
        }

    private:
        std::vector<T>& buffer_;
        T base_index_ = 0;
    };
}
