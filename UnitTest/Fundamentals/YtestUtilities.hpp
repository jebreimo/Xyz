//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 02.01.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "../../Xyz/Xyz.hpp"
#include "../../Submodules/Ytest/Ytest.hpp"

namespace Ytest {

    template <typename T, typename U, unsigned N>
    bool equal(const Xyz::Vector <T, N>& v, const Xyz::Vector <U, N>& u)
    {
        return Xyz::areEquivalent(v, u, 1e-9);
    }
}
