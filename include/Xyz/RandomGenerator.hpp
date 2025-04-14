//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-04-06.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <limits>
#include <random>

namespace Xyz
{
    std::default_random_engine& get_random_engine();

    template <typename IntT>
    class RandomIntGenerator
    {
    public:
        explicit RandomIntGenerator(
            IntT min = 0,
            IntT max = std::numeric_limits<IntT>::max())
            : m_engine(get_random_engine()), m_dist(min, max)
        {}

        explicit RandomIntGenerator(
            std::default_random_engine& engine,
            IntT min = 0,
            IntT max = std::numeric_limits<IntT>::max())
            : m_engine(get_random_engine()), m_dist(min, max)
        {}

        IntT operator()()
        {
            return m_dist(m_engine);
        }

    private:
        std::default_random_engine& m_engine;
        std::uniform_int_distribution<IntT> m_dist;
    };

    template <typename RealT>
    class RandomRealGenerator
    {
    public:
        explicit RandomRealGenerator(RealT min = 0.0, RealT max = 1.0)
            : m_engine(get_random_engine()),
              m_dist(min,
                     std::nextafter(max, std::numeric_limits<RealT>::max()))
        {}

        explicit RandomRealGenerator(std::default_random_engine& engine,
                                     RealT min = 0.0,
                                     RealT max = 1.0)
            : m_engine(get_random_engine()),
              m_dist(min,
                     std::nextafter(max, std::numeric_limits<RealT>::max()))
        {}

        RealT operator()()
        {
            return m_dist(m_engine);
        }
    private:
        std::default_random_engine& m_engine;
        std::uniform_real_distribution<RealT> m_dist;
    };
}
