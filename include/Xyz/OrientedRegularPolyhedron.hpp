//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Orientation.hpp"
#include "Vector.hpp"

namespace Xyz
{
    /**
     * @brief A regular polyhedron with a specific orientation.
     * @tparam T The value type of the polyhedron.
     */
    template <std::floating_point T>
    struct OrientedRegularPolyhedron
    {
        /**
         * The center of the polyhedron.
         */
        Vector<T, 3> center;
        /**
         * The radius of the circumscribed sphere.
         */
        T radius = 1;
        /**
         * The orientation of the polyhedron.
         */
        Orientation<T, 3> orientation = {};
    };
}
