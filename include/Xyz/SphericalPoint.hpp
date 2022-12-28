//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-26.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    struct SphericalPoint
    {
        SphericalPoint() = default;

        SphericalPoint(T radius, T azimuth, T polar)
            : radius(radius),
              azimuth(azimuth),
              polar(polar)
        {}

        T radius = {};
        T azimuth = {};
        T polar = {};
    };

    /**
     * @brief Returns the spherical point corresponding to the
     *  cartesian point @a p.
     */
    template <typename T>
    SphericalPoint<T> to_spherical(const Vector<T, 3>& p)
    {
        auto [x, y, z] = p;
        auto length = get_length(p);
        if (length == 0)
            return {};

        T theta;
        if (y > 0)
            theta = Constants<T>::PI / 2 - atan(x / y);
        else if (y < 0)
            theta = -Constants<T>::PI / 2 - atan(x / y);
        else if (x >= 0)
            theta = 0;
        else
            theta = Constants<T>::PI;

        auto phi = asin(z / length);
        return {length, theta, phi};
    }

    template <typename T>
    Vector<T, 3> to_cartesian(const SphericalPoint<T>& s)
    {
        return {s.radius * cos(s.azimuth) * cos(s.polar),
                s.radius * sin(s.azimuth) * cos(s.polar),
                s.radius * sin(s.polar)};
    }
}
