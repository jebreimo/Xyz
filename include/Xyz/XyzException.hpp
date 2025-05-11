//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-20.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <stdexcept>

namespace Xyz
{
    class XyzException : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
}

#define XYZ_THROW_3_(file, line, msg) \
    throw ::Xyz::XyzException(file ":" #line ": " msg)

#define XYZ_THROW_2_(file, line, msg) \
    XYZ_THROW_3_(file, line, msg)

#define XYZ_THROW(msg) \
    XYZ_THROW_2_(__FILE__, __LINE__, msg)
