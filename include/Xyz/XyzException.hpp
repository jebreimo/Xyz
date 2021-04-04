//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-20.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>
#include <string>

namespace Xyz
{
    class XyzException : public std::runtime_error
    {
    public:
        explicit XyzException(const std::string& message) noexcept
                : std::runtime_error(message)
        {}

        explicit XyzException(const char* message) noexcept
            : std::runtime_error(message)
        {}
    };
}

#define _XYZ_THROW_3(file, line, msg) \
    throw ::Xyz::XyzException(file ":" #line ": " msg)

#define _XYZ_THROW_2(file, line, msg) \
    _XYZ_THROW_3(file, line, msg)

#define XYZ_THROW(msg) \
    _XYZ_THROW_2(__FILE__, __LINE__, msg)
