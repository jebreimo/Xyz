//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-09-14.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>

#include "Vector.hpp"
#include "XyzException.hpp"

namespace Xyz
{
    enum class Axis : uint8_t
    {
        X = 0,
        Y = 1,
        Z = 2,
        NEG_X = 4,
        NEG_Y = 5,
        NEG_Z = 6
    };

    constexpr Axis operator-(Axis axis)
    {
        return static_cast<Axis>(static_cast<uint8_t>(axis) ^ 0x4);
    }

    constexpr bool is_negative(Axis axis)
    {
        return (static_cast<uint8_t>(axis) & 0x4) != 0;
    }

    template <typename T, unsigned N>
    struct AxisSwizzler;

    template <typename T>
    struct AxisSwizzler<T, 3>
    {
        uint8_t indexes[3];
        int8_t signs[3];

        Vector<T, 3> operator()(const Vector<T, 3>& v) const
        {
            return {
                static_cast<T>(signs[0]) * v[indexes[0]],
                static_cast<T>(signs[1]) * v[indexes[1]],
                static_cast<T>(signs[2]) * v[indexes[2]]
            };
        }
    };

    class AxisSystem
    {
    public:
        AxisSystem() = default;

        AxisSystem(Axis primary, Axis secondary, Axis up)
            : flags_(encode(primary, secondary, up))
        {}

        [[nodiscard]]
        std::tuple<Axis, Axis, Axis> get_axes() const
        {
            return decode(flags_);
        }

        template <typename T>
        [[nodiscard]]
        AxisSwizzler<T, 3> get_swizzler() const
        {
            const auto [primary, secondary, up] = decode(flags_);
            return {
                {static_cast<uint8_t>(primary) & 3,
                 static_cast<uint8_t>(secondary) & 3,
                 static_cast<uint8_t>(up) & 3},
                {is_negative(primary) ? -1 : 1,
                 is_negative(secondary) ? -1 : 1,
                 is_negative(up) ? -1 : 1}
            };
        }

    private:
        static uint8_t encode(Axis primary, Axis secondary, Axis up)
        {
            const auto p = static_cast<uint8_t>(primary);
            const auto s = static_cast<uint8_t>(secondary);
            const auto u = static_cast<uint8_t>(up);

            if ((p & 3) == (s & 3) || (p & 3) == (u & 3) || (s & 3) == (u & 3))
                XYZ_THROW("Axes must be different.");

            const uint8_t axes = (p & 3) * 2 + (1 + (s & 3) - (p & 3)) % 2;
            const uint8_t signs = (p & 4) | ((s & 4) << 1) | ((u & 4) << 2);
            return axes | signs << 1;
        }

        static std::tuple<Axis, Axis, Axis> decode(uint8_t flags)
        {
            auto axes = flags & 0x7;
            auto signs = (flags >> 3) & 0x7;

            const uint8_t unsigned_primary = axes / 2;
            const auto primary = static_cast<Axis>(unsigned_primary | ((signs & 1) << 2));
            axes %= 2;
            signs >>= 1;
            const auto unsigned_secondary = (unsigned_primary + 1 + axes) % 3;
            const auto secondary = static_cast<Axis>(unsigned_secondary | ((signs & 1) << 2));
            signs >>= 1;
            const auto up = static_cast<Axis>((unsigned_primary + unsigned_secondary + 1) % 3 | ((signs & 1) << 2));

            return {primary, secondary, up};
        }

        uint8_t flags_ = 0;
    };
}
