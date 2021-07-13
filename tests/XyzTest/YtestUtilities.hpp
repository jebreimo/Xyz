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
    bool equivalent(const Xyz::Vector<T, N>& u, const Xyz::Vector<T, N>& v,
                    T margin)
    {
        return Xyz::areEquivalent(u, v, margin);
    }

    template <typename T, unsigned N>
    bool equivalent(Xyz::Vector<T, N>& u, Xyz::Vector<T, N>& v, T margin)
    {
        return Xyz::areEquivalent(u, v, margin);
    }
}
