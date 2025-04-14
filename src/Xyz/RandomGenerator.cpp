//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-04-06.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/RandomGenerator.hpp"

namespace Xyz
{
    std::default_random_engine& get_random_engine()
    {
        static std::default_random_engine engine(
            [](){return std::random_device()();}());
        return engine;
    }
}
