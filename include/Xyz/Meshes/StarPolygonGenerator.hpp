//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-08-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Vector.hpp"

namespace Xyz
{
    class StarPolygonGenerator
    {
    public:
        double outerRadius() const
        {
            return m_OuterRadius;
        }

        StarPolygonGenerator& setOuterRadius(double outerRadius)
        {
            m_OuterRadius = outerRadius;
            return *this;
        }

        double innerRadius() const
        {
            return m_InnerRadius;
        }

        StarPolygonGenerator& setInnerRadius(double innerRadius)
        {
            m_InnerRadius = innerRadius;
            return *this;
        }

        double angle() const
        {
            return m_Angle;
        }

        StarPolygonGenerator& setAngle(double angle)
        {
            m_Angle = angle;
            return *this;
        }

        const Vector2d& center() const
        {
            return m_Center;
        }

        StarPolygonGenerator& setCenter(const Vector2d& center)
        {
            m_Center = center;
            return *this;
        }

        size_t numberOfPoints() const
        {
            return m_NumberOfPoints;
        }

        StarPolygonGenerator& setNumberOfPoints(size_t numberOfPoints)
        {
            m_NumberOfPoints = numberOfPoints;
            return *this;
        }

        bool isClosed() const
        {
            return m_IsClosed;
        }

        StarPolygonGenerator& setIsClosed(bool isClosed)
        {
            m_IsClosed = isClosed;
            return *this;
        }

        template <typename T>
        std::vector<Vector<T, 2>> generate() const
        {
            if (m_NumberOfPoints < 2)
                return {};
            std::vector<Vector<T, 2>> result;
            result.reserve(2 * (m_NumberOfPoints + (m_IsClosed ? 1 : 0)));
            for (size_t i = 0; i < m_NumberOfPoints; ++i)
            {
                auto angle1 = m_Angle - i * PI_64 / m_NumberOfPoints;
                result.emplace_back({T(m_Center[0] + m_OuterRadius * cos(angle1)),
                                     T(m_Center[1] + m_OuterRadius * sin(angle1))});
                auto angle2 = m_Angle - (i + 0.5) * PI_64 / m_NumberOfPoints;
                result.emplace_back({T(m_Center[0] + m_InnerRadius * cos(angle2)),
                                     T(m_Center[1] + m_InnerRadius * sin(angle2))});
            }
            if (m_IsClosed && !result.empty())
                result.push_back(result.front());
            return result;
        }

    private:
        double m_OuterRadius = 1.0;
        double m_InnerRadius = 0.5;
        double m_Angle = 0.0;
        Vector2d m_Center;
        size_t m_NumberOfPoints = 4;
        bool m_IsClosed = false;
    };
}
