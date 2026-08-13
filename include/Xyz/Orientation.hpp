//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include "Quaternion.hpp"
#include "RotationMatrix.hpp"
#include "Utilities.hpp"

namespace Xyz
{
    template <std::floating_point T, unsigned N>
    struct Orientation;

    template <std::floating_point T>
    struct Orientation<T, 2>
    {
        T angle;
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
    std::tuple<Vector<T, 2>, Vector<T, 2>>
    get_vectors(const Orientation<T, 2>& o)
    {
        const auto c = std::cos(o.angle);
        const auto s = std::sin(o.angle);
        return {{c, s}, {-s, c}};
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
        constexpr auto pi = Constants<double>::PI;
        return {o.angle + (o.angle < 0 ? pi : -pi)};
    }

    namespace linear
    {
        /**
         * @brief Returns the rotation matrix that corresponds to @a o.
         */
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 2, 2> to_matrix(const Orientation<T, 2>& o)
        {
            return rotate2(o.angle);
        }
    }

    namespace affine
    {
        /**
         * @brief Returns the transformation matrix that rotates by @a o and
         *  then translates by @a offset.
         */
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 3, 3>
        to_matrix(const Orientation<T, 2>& o,
                  const Vector<std::type_identity_t<T>, 2>& offset = {})
        {
            const auto c = std::cos(o.angle);
            const auto s = std::sin(o.angle);
            return {
                c, -s, offset[0],
                s, c, offset[1],
                0, 0, 1
            };
        }
    }

    template <std::floating_point T>
    struct Orientation<T, 3>
    {
        /**
         * @brief Rotation around z-axis, counterclockwise is positive
         */
        T yaw = {};
        /**
         * @brief Rotation around y-axis, "down" is positive
         */
        T pitch = {};
        /**
         * @brief Rotation around x-axis, "clockwise" is positive
         */
        T roll = {};
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
        auto c_y = std::cos(o.yaw);
        auto s_y = std::sin(o.yaw);
        auto c_p = std::cos(o.pitch);
        auto s_p = std::sin(o.pitch);

        return {
            c_y * c_p,
            s_y * c_p,
            -s_p
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_y_vector(const Orientation<T, 3>& o)
    {
        auto c_y = std::cos(o.yaw);
        auto s_y = std::sin(o.yaw);
        auto c_p = std::cos(o.pitch);
        auto s_p = std::sin(o.pitch);
        auto c_r = std::cos(o.roll);
        auto s_r = std::sin(o.roll);

        return {
            c_y * s_p * s_r - s_y * c_r,
            s_y * s_p * s_r + c_y * c_r,
            c_p * s_r
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_z_vector(const Orientation<T, 3>& o)
    {
        auto c_y = std::cos(o.yaw);
        auto s_y = std::sin(o.yaw);
        auto c_p = std::cos(o.pitch);
        auto s_p = std::sin(o.pitch);
        auto c_r = std::cos(o.roll);
        auto s_r = std::sin(o.roll);

        return {
            c_y * s_p * c_r + s_y * s_r,
            s_y * s_p * c_r - c_y * s_r,
            c_p * c_r
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    std::tuple<Vector<T, 3>, Vector<T, 3>, Vector<T, 3>>
    get_vectors(const Orientation<T, 3>& o)
    {
        auto c_y = std::cos(o.yaw);
        auto s_y = std::sin(o.yaw);
        auto c_p = std::cos(o.pitch);
        auto s_p = std::sin(o.pitch);
        auto c_r = std::cos(o.roll);
        auto s_r = std::sin(o.roll);

        return {
            Vector<T, 3>{c_y * c_p, s_y * c_p, -s_p},
            Vector<T, 3>{c_y * s_p * s_r - s_y * c_r,
                         s_y * s_p * s_r + c_y * c_r,
                         c_p * s_r},
            Vector<T, 3>{c_y * s_p * c_r + s_y * s_r,
                         s_y * s_p * c_r - c_y * s_r,
                         c_p * c_r}
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

    /**
     * @brief Returns the unit quaternion that corresponds to @a o.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> to_quaternion(const Orientation<T, 3>& o)
    {
        const auto c_y = std::cos(o.yaw / 2);
        const auto s_y = std::sin(o.yaw / 2);
        const auto c_p = std::cos(o.pitch / 2);
        const auto s_p = std::sin(o.pitch / 2);
        const auto c_r = std::cos(o.roll / 2);
        const auto s_r = std::sin(o.roll / 2);

        // The rotations are applied in the order roll, pitch, yaw, so the
        // quaternion is the product yaw * pitch * roll.
        return {
            c_r * c_p * c_y + s_r * s_p * s_y,
            s_r * c_p * c_y - c_r * s_p * s_y,
            c_r * s_p * c_y + s_r * c_p * s_y,
            c_r * c_p * s_y - s_r * s_p * c_y
        };
    }

    /**
     * @brief Returns the orientation that corresponds to @a q.
     *
     * The result is normalized, i.e. pitch is in the range [-PI/2, PI/2] and
     * yaw and roll are in the range [-PI, PI]. When the pitch is straight up
     * or down, yaw and roll represent the same rotation, and the whole
     * rotation is assigned to yaw.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Orientation<T, 3> to_orientation(const Quaternion<T>& q)
    {
        const auto u = normalize(q);
        const auto [x, y, z] = u.v;
        const auto w = u.w;

        const auto sin_pitch = T(2) * (w * y - z * x);
        if (std::abs(sin_pitch) >= 1 - Margin<T>::DEFAULT)
        {
            // Gimbal lock: the roll and yaw axes coincide, so pick roll = 0.
            constexpr auto pi = Constants<T>::PI;
            return {
                T(2) * std::atan2(z, w),
                sin_pitch > 0 ? pi / 2 : -pi / 2,
                0
            };
        }

        return {
            std::atan2(T(2) * (w * z + x * y), T(1) - T(2) * (y * y + z * z)),
            std::asin(sin_pitch),
            std::atan2(T(2) * (w * x + y * z), T(1) - T(2) * (x * x + y * y))
        };
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
                  const Vector<std::type_identity_t<T>, 3>& offset = {})
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
