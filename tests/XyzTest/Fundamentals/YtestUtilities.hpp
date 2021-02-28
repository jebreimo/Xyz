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

namespace Ytest
{
    template <typename T, unsigned N>
    bool equal(const Xyz::Vector<T, N>& u, const Xyz::Vector<T, N>& v)
    {
        return Xyz::areEquivalent(u, v, 1e-9);
    }

    inline bool equal(const Xyz::Vector<double, 4>& u,
                      const Xyz::Vector<double, 4>& v)
    {
        return Xyz::areEquivalent(u, v, 1e-9);
    }
}
