//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Orientation.hpp"

namespace Xyz
{
    /**
     * @brief A 2D placement defined by an origin point and an orientation.
     * @tparam T The value type of the placement.
     */
    template <std::floating_point T, unsigned N>
    struct Placement
    {
        Vector<T, N> origin; ///< The origin point of the placement.
        Orientation<T, N> orientation; ///< The orientation of the placement.
    };

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator==(const Placement<T, N>& lhs, const Placement<T, N>& rhs)
    {
        return lhs.origin == rhs.origin && lhs.orientation == rhs.orientation;
    }

    template <std::floating_point T, unsigned N>
    [[nodiscard]]
    bool operator!=(const Placement<T, N>& lhs, const Placement<T, N>& rhs)
    {
        return !(lhs == rhs);
    }

    template <std::floating_point T, unsigned N>
    std::ostream& operator<<(std::ostream& os, const Placement<T, N>& rect)
    {
        return os << '{' << rect.origin << ", " << rect.orientation << '}';
    }

    using Placement2D = Placement<float, 2>;
    using Placement3D = Placement<float, 3>;
}
