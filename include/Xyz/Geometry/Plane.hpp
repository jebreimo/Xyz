//****************************************************************************
// Copyright © 2019 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2019-07-23.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Plane
    {
    public:
        Plane() = default;

        Plane(const Vector<T, 3>& origin,
              const Vector<T, 3>& axis1,
              const Vector<T, 3>& axis2)
                : m_Origin(origin),
                  m_Axis1(axis1),
                  m_Axis2(axis2),
                  m_Normal(cross(m_Axis1, m_Axis2))
        {
            if (std::abs(m_Axis1 * m_Axis2) > 1e-12)
                XYZ_THROW("A plane's axes must be orthogonal");
        }

        Plane(const Vector<T, 3>& origin,
              const Vector<T, 3>& axis1,
              const Vector<T, 3>& axis2,
              const Vector<T, 3>& normal)
                : m_Origin(origin),
                  m_Axis1(axis1),
                  m_Axis2(axis2),
                  m_Normal(normal)
        {
            if (std::abs(m_Axis1 * m_Axis2) > 1e-12)
                XYZ_THROW("A plane's axes must be orthogonal");
        }

        const Vector<T, 3>& origin() const
        {
            return m_Origin;
        }

        const Vector<T, 3>& axis1() const
        {
            return m_Axis1;
        }

        const Vector<T, 3>& axis2() const
        {
            return m_Axis2;
        }

        const Vector<T, 3>& normal() const
        {
            return m_Normal;
        }
    private:
        Vector<T, 3> m_Origin;
        Vector<T, 3> m_Axis1;
        Vector<T, 3> m_Axis2;
        Vector<T, 3> m_Normal;
    };

    template <typename T>
    Plane<T> makePlaneFromNormal(const Vector<T, 3>& origin,
                                 const Vector<T, 3>& normal)
    {
        auto unit = getUnit(normal);
        //auto xLength = unit * makeVector<T>(1, 0, 0);
        //auto yLength = unit * makeVector<T>(0, 1, 0);
        //auto zLength = unit * makeVector<T>(0, 0, 1);
        Vector<T, 3> v;
        Vector<T, 3> major;
        if (getZ(unit) >= getX(unit) && getZ(unit) >= getY(unit))
        {
            v = {0, 0, 1};
            major = {getZ(unit) >= 0 ? 1 : -1, 0, 0};
        }
        else if (getY(unit) >= getX(unit))
        {
            v = {0, 1, 0};
            major = {getY(unit) >= 0 ? 1 : -1, 0, 0};
        }
        else
        {
            v = {1, 0, 0};
            major = {0, getX(unit) >= 0 ? 1 : -1, 0};
        }
        if (std::abs(v * unit) > 1e-9)
            major = unit(cross(v, unit));
        auto minor = cross(unit, major);
        return {origin, major, minor, unit};
    }

    template <typename T>
    Vector<T, 3> fromPlanarPoint(const Plane<T>& plane,
                                 const Vector<T, 3>& point)
    {
        return plane.origin()
               + plane.axis1() * point[0]
               + plane.axis2() * point[1]
               + plane.axis3() * point[2];
    }

    //template <typename T>
    //Vector<T, 3> toPlanarPoint(const Plane<T>& plane,
    //                           const Vector<T, 3>& point)
    //{
    //    auto p = point - plane.origin();
    //    return
    //}
}
