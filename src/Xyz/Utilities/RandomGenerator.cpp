//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-04-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/Utilities/RandomGenerator.hpp"

namespace Xyz
{
    std::default_random_engine& getRandomEngine()
    {
        static std::default_random_engine engine;
        static bool initialized = false;
        if (!initialized)
        {
            std::random_device rd;
            engine.seed(rd());
        }
        return engine;
    }
}
