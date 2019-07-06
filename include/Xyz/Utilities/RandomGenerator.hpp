//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-04-06.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <limits>
#include <random>

namespace Xyz
{
    std::default_random_engine& getRandomEngine();

    template <typename IntT>
    class RandomIntGenerator
    {
    public:
        RandomIntGenerator(IntT min = 0,
                           IntT max = std::numeric_limits<IntT>::max())
            : m_Engine(getRandomEngine()), m_Dist(min, max)
        {}

        RandomIntGenerator(std::default_random_engine& randomEngine,
                           IntT min = 0,
                           IntT max = std::numeric_limits<IntT>::max())
            : m_Engine(getRandomEngine()), m_Dist(min, max)
        {}

        IntT operator()()
        {
            return m_Dist(m_Engine);
        }

    private:
        std::default_random_engine& m_Engine;
        std::uniform_int_distribution<IntT> m_Dist;
    };

    template <typename RealT>
    class RandomRealGenerator
    {
    public:
        RandomRealGenerator(RealT min = 0.0, RealT max = 1.0)
            : m_Engine(getRandomEngine()),
              m_Dist(min,
                     std::nextafter(max, std::numeric_limits<RealT>::max()))
        {}

        RandomRealGenerator(std::default_random_engine& randomEngine,
                            RealT min = 0.0,
                            RealT max = 1.0)
            : m_Engine(getRandomEngine()),
              m_Dist(min,
                     std::nextafter(max, std::numeric_limits<RealT>::max()))
        {}

        RealT operator()()
        {
            return m_Dist(m_Engine);
        }
    private:
        std::default_random_engine& m_Engine;
        std::uniform_real_distribution<RealT> m_Dist;
    };
}
