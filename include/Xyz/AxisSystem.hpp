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
        NEGATIVE_X = 4,
        NEGATIVE_Y = 5,
        NEGATIVE_Z = 6
    };

    constexpr Axis operator-(Axis axis)
    {
        return static_cast<Axis>(static_cast<uint8_t>(axis) ^ 0x4);
    }

    constexpr bool is_negative(Axis axis)
    {
        return (static_cast<uint8_t>(axis) & 0x4) != 0;
    }

    template<typename T, unsigned N>
    struct AxisSwizzler;

    template<typename T>
    struct AxisSwizzler<T, 3>
    {
        unsigned indexes[3];
        T signs[3];

        Vector<T, 3> operator()(const Vector<T, 3>& v) const
        {
            return {
                signs[0] * v[indexes[0]],
                signs[1] * v[indexes[1]],
                signs[2] * v[indexes[2]]
            };
        }
    };

    class AxisSystem
    {
    public:
        AxisSystem() = default;

        AxisSystem(Axis forward, Axis side, Axis up)
            : flags_(encode(forward, side, up))
        {
        }

        [[nodiscard]]
        std::tuple<Axis, Axis, Axis> get_axes() const
        {
            return decode(flags_);
        }

        template<typename T>
        [[nodiscard]]
        AxisSwizzler<T, 3> get_swizzler() const
        {
            const auto [forward, side, up] = decode(flags_);

            const auto f = static_cast<unsigned>(forward) & 3;
            const auto s = static_cast<unsigned>(side) & 3;
            const auto u = static_cast<unsigned>(up) & 3;
            AxisSwizzler<T, 3> swizzler;
            swizzler.indexes[f] = 0;
            swizzler.indexes[s] = 1;
            swizzler.indexes[u] = 2;
            swizzler.signs[f] = is_negative(forward) ? T(-1) : T(1);
            swizzler.signs[s] = is_negative(side) ? T(-1) : T(1);
            swizzler.signs[u] = is_negative(up) ? T(-1) : T(1);

            return swizzler;
        }

    private:
        static uint8_t encode(Axis forward, Axis side, Axis up)
        {
            const auto f = static_cast<uint8_t>(forward);
            const auto s = static_cast<uint8_t>(side);
            const auto u = static_cast<uint8_t>(up);

            if ((f & 3) == (s & 3) || (f & 3) == (u & 3) || (s & 3) == (u & 3))
                XYZ_THROW("Forward, side, and up axes must be different.");

            const uint8_t axes = (f & 3) * 2 + (2 + (s & 3) - (f & 3)) % 3;
            const uint8_t signs = (f & 4) | ((s & 4) << 1) | ((u & 4) << 2);
            return axes | signs << 1;
        }

        static std::tuple<Axis, Axis, Axis> decode(uint8_t flags)
        {
            const uint8_t axes = flags & 0x7;
            const uint8_t signs = (flags >> 3) & 0x7;

            const uint8_t unsigned_f = axes / 2;
            const uint8_t unsigned_s = (unsigned_f + 1 + axes % 2) % 3;
            const auto f = static_cast<Axis>(unsigned_f | ((signs & 1) << 2));
            const auto s = static_cast<Axis>(unsigned_s | ((signs & 2) << 1));
            const auto u = static_cast<Axis>(3 - (unsigned_f + unsigned_s) | (signs & 4));

            return {f, s, u};
        }

        uint8_t flags_ = 0;
    };
}
