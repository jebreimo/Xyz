//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 02.01.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Xyz/Xyz.hpp"
#include "Ytest/Ytest.hpp"

namespace Ytest {

    template <typename T, unsigned N>
    bool equal(const Xyz::Vector<T, N>& v, const Xyz::Vector<T, N>& u)
    {
        return Xyz::equivalent(v, u, 1e-9);
    }
}
