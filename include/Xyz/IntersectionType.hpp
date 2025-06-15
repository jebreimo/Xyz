//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-06-15.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <iosfwd>

namespace Xyz
{
    enum class IntersectionType
    {
        /* Lines, line segments or planes are parallel, but not co-linear.
         */
        NON_INTERSECTING,
        /* Lines, line segments or planes are intersecting.
         */
        INTERSECTING,
        /* Lines, line segments or planes are overlapping.
         */
        OVERLAPPING,
        /* Line segments are colinear and may or may not overlap.
         */
        COLINEAR
    };

    std::ostream& operator<<(std::ostream& os, IntersectionType e);
}
