//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-06.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Placement.hpp"

namespace Xyz
{
    /**
     * Class for expressing rectangular cuboids.
     * @tparam T
     */
    template <std::floating_point T>
    struct Cuboid
    {
        Placement<T, 3> placement;
        Vector<T, 3> size;
    };
}
