//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-06-15.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/IntersectionType.hpp"
#include <ostream>

namespace Xyz
{
#define CASE_STREAM_ENUM(name) \
    case name: return os << #name

    std::ostream& operator<<(std::ostream& os, IntersectionType e)
    {
        switch (e)
        {
        CASE_STREAM_ENUM(IntersectionType::NON_INTERSECTING);
        CASE_STREAM_ENUM(IntersectionType::INTERSECTING);
        CASE_STREAM_ENUM(IntersectionType::OVERLAPPING);
        CASE_STREAM_ENUM(IntersectionType::COLINEAR);
        default: return os << "Unknown value.";
        }
    }
}
