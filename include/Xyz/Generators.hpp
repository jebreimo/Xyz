//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-01-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    class CircleIterator
    {
    public:
        CircleIterator() = default;

        explicit CircleIterator(unsigned count)
            : m_count(count)
        {}

        CircleIterator(unsigned count, double start_angle, double delta_angle)
            : m_count(count),
              m_start_angle(start_angle),
              m_delta_angle(delta_angle)
        {}

        CircleIterator& operator++()
        {
            ++m_count;
            return *this;
        }

        CircleIterator operator++(int)
        {
            auto it = *this;
            ++m_count;
            return it;
        }

        Vector<T, 2> operator*() const
        {
            auto angle = m_start_angle + T(m_count * m_delta_angle);
            return {std::cos(angle), std::sin(angle)};
        }

        friend bool operator==(const CircleIterator& a,
                               const CircleIterator& b)
        {
            return a.m_count == b.m_count;
        }

        friend bool operator!=(const CircleIterator& a,
                               const CircleIterator& b)
        {
            return a.m_count != b.m_count;
        }
    private:
        unsigned m_count = 0;
        T m_start_angle = 0;
        T m_delta_angle = 0;
    };

    template <typename T>
    class CircleGenerator
    {
    public:
        explicit CircleGenerator(unsigned number_of_points)
            : m_points(number_of_points)
        {}

        CircleGenerator& start_angle(double value)
        {
            m_start_angle = value;
            return *this;
        }

        CircleGenerator& ccw(bool value)
        {
            m_ccw = value;
            return *this;
        }

        CircleGenerator& repeat_first_point(bool value)
        {
            m_repeat_first_point = value;
            return *this;
        }

        CircleIterator<T> begin() const
        {
            T delta_angle = 2 * Constants<T>::PI / m_points;
            if (!m_ccw)
                delta_angle = -delta_angle;
            return {0, m_start_angle, delta_angle};
        }

        CircleIterator<T> end() const
        {
            return {m_points + (m_repeat_first_point ? 1 : 0), 0.0, 0.0};
        }
    private:
        unsigned m_points;
        double m_start_angle = 0;
        bool m_ccw = true;
        bool m_repeat_first_point = false;
    };
}
