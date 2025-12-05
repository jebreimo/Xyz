//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include "RotationMatrix.hpp"
#include "Utilities.hpp"

namespace Xyz
{
    template <std::floating_point T, unsigned N>
    struct Orientation;

    template <std::floating_point T>
    struct Orientation<T, 2>
    {
        Orientation() = default;

        // ReSharper disable once CppNonExplicitConvertingConstructor
        Orientation(T angle)
            : angle(angle)
        {}

        union
        {
            T angle;
            T yaw;
        };
    };

    template <std::floating_point T>
    [[nodiscard]]
    bool operator==(const Orientation<T, 2>& lhs, const Orientation<T, 2>& rhs)
    {
        return lhs.angle == rhs.angle;
    }

    template <std::floating_point T>
    [[nodiscard]]
    bool operator!=(const Orientation<T, 2>& lhs, const Orientation<T, 2>& rhs)
    {
        return !(lhs == rhs);
    }

    template <std::floating_point T>
    [[nodiscard]]
    std::ostream& operator<<(std::ostream& os, const Orientation<T, 2>& o)
    {
        return os << "{angle: " << o.angle << "}";
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 2> get_x_vector(const Orientation<T, 2>& o)
    {
        return {std::cos(o.angle), std::sin(o.angle)};
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 2> get_y_vector(const Orientation<T, 2>& o)
    {
        return {-std::sin(o.angle), std::cos(o.angle)};
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 2> normalize(const Orientation<T, 2>& o)
    {
        return {to_signed_principal_angle(o.angle)};
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 2> reverse(const Orientation<T, 2>& o)
    {
        constexpr auto pi = Xyz::Constants<double>::PI;
        return {o.angle + (o.angle < 0 ? pi : -pi)};
    }

    namespace linear
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 2, 2> to_matrix(const Orientation<T, 2>& o)
        {
            return rotate2(o.angle);
        }
    }

    namespace affine
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 3, 3> to_matrix(const Orientation<T, 2>& o)
        {
            return rotate2(o.angle);
        }
    }

    template <std::floating_point T>
    struct Orientation<T, 3>
    {
        Orientation() = default;

        explicit Orientation(T yaw, T pitch = 0, T roll = 0)
            : yaw(yaw), pitch(pitch), roll(roll)
        {}

        /**
         * @brief Rotation around z-axis, counterclockwise is positive
         */
        T yaw;
        /**
         * @brief Rotation around y-axis, "down" is positive
         */
        T pitch;
        /**
         * @brief Rotation around x-axis, "clockwise" is positive
         */
        T roll;
    };

    template <std::floating_point T>
    [[nodiscard]]
    bool operator==(const Orientation<T, 3>& lhs, const Orientation<T, 3>& rhs)
    {
        return lhs.yaw == rhs.yaw && lhs.pitch == rhs.pitch && lhs.roll == rhs.roll;
    }

    template <std::floating_point T>
    [[nodiscard]]
    bool operator!=(const Orientation<T, 3>& lhs, const Orientation<T, 3>& rhs)
    {
        return !(lhs == rhs);
    }

    template <std::floating_point T>
    [[nodiscard]]
    std::ostream& operator<<(std::ostream& os, const Orientation<T, 3>& o)
    {
        return os << "{yaw: " << o.yaw
            << ", pitch: " << o.pitch
            << ", roll: " << o.roll << "}";
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_x_vector(const Orientation<T, 3>& o)
    {
        auto c_a = std::cos(o.yaw);
        auto s_a = std::sin(o.yaw);
        auto c_b = std::cos(o.pitch);
        auto s_b = std::sin(o.pitch);

        return {
            c_a * c_b,
            s_a * c_b,
            s_b
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_y_vector(const Orientation<T, 3>& o)
    {
        auto c_a = std::cos(o.yaw);
        auto s_a = std::sin(o.yaw);
        auto c_b = std::cos(o.pitch);
        auto s_b = std::sin(o.pitch);
        auto c_c = std::cos(o.roll);
        auto s_c = std::sin(o.roll);

        return {
            c_a * s_b * s_c - s_a * c_c,
            s_a * s_b * s_c + c_a * c_c,
            c_b * s_c
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_z_vector(const Orientation<T, 3>& o)
    {
        auto c_a = std::cos(o.yaw);
        auto s_a = std::sin(o.yaw);
        auto c_b = std::cos(o.pitch);
        auto s_b = std::sin(o.pitch);
        auto c_c = std::cos(o.roll);
        auto s_c = std::sin(o.roll);

        return {
            c_a * s_b * c_c + s_a * s_c,
            s_a * s_b * c_c - c_a * s_c,
            c_b * c_c
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 3> normalize(const Orientation<T, 3>& o)
    {
        auto result = Orientation<T, 3>{
            to_signed_principal_angle(o.yaw),
            to_signed_principal_angle(o.pitch),
            to_signed_principal_angle(o.roll)
        };

        constexpr auto pi = Constants<T>::PI;
        if (result.pitch < -pi / 2 || pi / 2 < result.pitch)
        {
            result.pitch = (result.pitch >= 0 ? pi : -pi) - result.pitch;
            result.yaw += result.yaw <= 0 ? pi : -pi;
            result.roll += result.roll <= 0 ? pi : -pi;
        }

        return result;
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 3> reverse(const Orientation<T, 3>& o)
    {
        constexpr auto pi = Constants<T>::PI;
        return Orientation<T, 3>{
            o.yaw + (o.yaw < 0 ? pi : -pi),
            o.pitch + (o.pitch < 0 ? pi : -pi),
            o.roll + (o.roll < 0 ? pi : -pi)
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 3> to_orientation(const Vector<T, 3>& v)
    {
        const auto u = normalize(v);
        return Orientation<T, 3>(
            std::atan2(u.y(), u.x()),
            std::asin(-u.z()),
            0
        );
    }

    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 3> to_orientation(const Vector<T, 3>& longitudinal,
                                     const Vector<T, 3>& lateral)
    {
        const auto lon = normalize(longitudinal);
        const auto lat = normalize(lateral);
        const auto up = cross(lon, lat);
        return Orientation<T, 3>(
            std::atan2(lon.y(), lon.x()),
            std::asin(lon.z()),
            std::atan2(lat.z(), up.z())
        );
    }

    namespace linear
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 3, 3> to_matrix(const Orientation<T, 3>& o)
        {
            auto c_a = std::cos(o.yaw);
            auto s_a = std::sin(o.yaw);
            auto c_b = std::cos(o.pitch);
            auto s_b = std::sin(o.pitch);
            auto c_c = std::cos(o.roll);
            auto s_c = std::sin(o.roll);
            return {
                c_a * c_b, c_a * s_b * s_c - s_a * c_c, c_a * s_b * c_c + s_a * s_c,
                s_a * c_b, s_a * s_b * s_c + c_a * c_c, s_a * s_b * c_c - c_a * s_c,
                -s_b, c_b * s_c, c_b * c_c
            };
        }
    }

    namespace affine
    {
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 4, 4>
        to_matrix(const Orientation<T, 3>& o,
                  const Vector<std::type_identity<T>, 3>& offset = {})
        {
            auto c_a = std::cos(o.yaw);
            auto s_a = std::sin(o.yaw);
            auto c_b = std::cos(o.pitch);
            auto s_b = std::sin(o.pitch);
            auto c_c = std::cos(o.roll);
            auto s_c = std::sin(o.roll);
            return {
                c_a * c_b, c_a * s_b * s_c - s_a * c_c, c_a * s_b * c_c + s_a * s_c, offset.x,
                s_a * c_b, s_a * s_b * s_c + c_a * c_c, s_a * s_b * c_c - c_a * s_c, offset.y,
                -s_b, c_b * s_c, c_b * c_c, offset.z,
                0, 0, 0, 1
            };
        }
    }

    using Orientation2F = Orientation<float, 2>;
    using Orientation2D = Orientation<double, 2>;
    using Orientation3F = Orientation<float, 3>;
    using Orientation3D = Orientation<double, 3>;
}
