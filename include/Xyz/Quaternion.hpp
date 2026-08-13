//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-12.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <concepts>
#include "Constants.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "XyzException.hpp"

namespace Xyz
{
    /**
     * @brief A quaternion, i.e. a scalar and a vector part.
     *
     * Quaternions are used to represent rotations in 3D space. Only unit
     * quaternions represent rotations, but the arithmetic operations work
     * for any quaternion.
     *
     * The rotations follow the same convention as the rest of the library:
     * vectors are columns, so a rotation of @a v by @a q is q * v * q⁻¹, and
     * the product q1 * q2 is the rotation q2 followed by the rotation q1.
     */
    template <std::floating_point T>
    struct Quaternion
    {
        using ValueType = T;

        /**
         * @brief Creates the identity quaternion, i.e. no rotation at all.
         */
        constexpr Quaternion() noexcept
            : w(1),
              v()
        {}

        constexpr Quaternion(T w, const Vector<T, 3>& v) noexcept
            : w(w),
              v(v)
        {}

        constexpr Quaternion(T w, T x, T y, T z) noexcept
            : w(w),
              v(x, y, z)
        {}

        /**
         * @brief The scalar part, i.e. cos(angle / 2) in a unit quaternion.
         */
        T w;

        /**
         * @brief The vector part, i.e. axis * sin(angle / 2) in a unit
         *  quaternion.
         */
        Vector<T, 3> v;
    };

    template <std::floating_point T>
    [[nodiscard]]
    constexpr bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    {
        return lhs.w == rhs.w && lhs.v == rhs.v;
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <std::floating_point T>
    std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q)
    {
        return os << "{w: " << q.w << ", v: " << q.v << "}";
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator-(const Quaternion<T>& q)
    {
        return {-q.w, -q.v};
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator+(const Quaternion<T>& lhs,
                                      const Quaternion<T>& rhs)
    {
        return {lhs.w + rhs.w, lhs.v + rhs.v};
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator-(const Quaternion<T>& lhs,
                                      const Quaternion<T>& rhs)
    {
        return {lhs.w - rhs.w, lhs.v - rhs.v};
    }

    /**
     * @brief Returns the Hamilton product of @a lhs and @a rhs.
     *
     * The result is the rotation @a rhs followed by the rotation @a lhs.
     */
    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator*(const Quaternion<T>& lhs,
                                      const Quaternion<T>& rhs)
    {
        return {
            lhs.w * rhs.w - dot(lhs.v, rhs.v),
            lhs.w * rhs.v + rhs.w * lhs.v + cross(lhs.v, rhs.v)
        };
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator*(const Quaternion<T>& q,
                                      std::type_identity_t<T> scalar)
    {
        return {q.w * scalar, q.v * scalar};
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator*(std::type_identity_t<T> scalar,
                                      const Quaternion<T>& q)
    {
        return q * scalar;
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> operator/(const Quaternion<T>& q,
                                      std::type_identity_t<T> scalar)
    {
        return {q.w / scalar, q.v / scalar};
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr Quaternion<T> conjugate(const Quaternion<T>& q)
    {
        return {q.w, -q.v};
    }

    template <std::floating_point T>
    [[nodiscard]]
    constexpr T get_length_squared(const Quaternion<T>& q)
    {
        return q.w * q.w + get_length_squared(q.v);
    }

    template <std::floating_point T>
    [[nodiscard]]
    T get_length(const Quaternion<T>& q)
    {
        return std::sqrt(get_length_squared(q));
    }

    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> normalize(const Quaternion<T>& q)
    {
        return q / get_length(q);
    }

    /**
     * @brief Returns the quaternion that undoes the rotation @a q.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> invert(const Quaternion<T>& q)
    {
        const auto length_squared = get_length_squared(q);
        if (length_squared == 0)
            XYZ_THROW("The quaternion is not invertible.");
        return conjugate(q) / length_squared;
    }

    template <std::floating_point T>
    [[nodiscard]]
    bool are_equal(const Quaternion<T>& p, const Quaternion<T>& q,
                   std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return get_length_squared(p - q) <= margin;
    }

    /**
     * @brief Returns true if @a p and @a q represent the same rotation.
     *
     * A quaternion and its negation are different quaternions, but they
     * rotate vectors in exactly the same way.
     */
    template <std::floating_point T>
    [[nodiscard]]
    bool are_equivalent(const Quaternion<T>& p, const Quaternion<T>& q,
                        std::type_identity_t<T> margin = Margin<T>::DEFAULT)
    {
        return are_equal(p, q, margin) || are_equal(p, -q, margin);
    }

    /**
     * @brief Returns @a v rotated by @a q.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> rotate(const Quaternion<T>& q, const Vector<T, 3>& v)
    {
        // The textbook expression is q * Quaternion(0, v) * invert(q), but
        // this form avoids the two full quaternion products, and works for
        // non-unit quaternions as well.
        const auto t = T(2) * cross(q.v, v);
        return v + (q.w * t + cross(q.v, t)) / get_length_squared(q);
    }

    /**
     * @brief Returns the unit quaternion that rotates @a angle radians
     *  counterclockwise around @a axis.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> make_quaternion(std::type_identity_t<T> angle,
                                  const Vector<T, 3>& axis)
    {
        return {std::cos(angle / 2), normalize(axis) * std::sin(angle / 2)};
    }

    /**
     * @brief Returns the rotation angle of @a q in the range [0, 2*PI).
     */
    template <std::floating_point T>
    [[nodiscard]]
    T get_angle(const Quaternion<T>& q)
    {
        return T(2) * std::atan2(get_length(q.v), q.w);
    }

    /**
     * @brief Returns the unit length rotation axis of @a q.
     *
     * Returns the x-axis if @a q has no rotation, as the axis is arbitrary
     * in that case.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Vector<T, 3> get_axis(const Quaternion<T>& q)
    {
        const auto length = get_length(q.v);
        if (length == 0)
            return {1, 0, 0};
        return q.v / length;
    }

    namespace linear
    {
        /**
         * @brief Returns the rotation matrix that corresponds to @a q.
         */
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 3, 3> to_matrix(const Quaternion<T>& q)
        {
            const auto [x, y, z] = q.v;
            // Scaling by the squared length makes non-unit quaternions
            // produce a rotation matrix rather than a scaled one.
            const auto s = T(2) / get_length_squared(q);
            const auto xx = s * x * x, yy = s * y * y, zz = s * z * z;
            const auto xy = s * x * y, xz = s * x * z, yz = s * y * z;
            const auto wx = s * q.w * x, wy = s * q.w * y, wz = s * q.w * z;
            return {
                T(1) - yy - zz, xy - wz, xz + wy,
                xy + wz, T(1) - xx - zz, yz - wx,
                xz - wy, yz + wx, T(1) - xx - yy
            };
        }
    }

    namespace affine
    {
        /**
         * @brief Returns the transformation matrix that rotates by @a q and
         *  then translates by @a offset.
         */
        template <std::floating_point T>
        [[nodiscard]]
        Matrix<T, 4, 4> to_matrix(const Quaternion<T>& q,
                                  const Vector<std::type_identity_t<T>, 3>& offset = {})
        {
            const auto m = linear::to_matrix(q);
            return {
                m[0, 0], m[0, 1], m[0, 2], offset[0],
                m[1, 0], m[1, 1], m[1, 2], offset[1],
                m[2, 0], m[2, 1], m[2, 2], offset[2],
                0, 0, 0, 1
            };
        }
    }

    /**
     * @brief Returns the unit quaternion that corresponds to the rotation
     *  matrix @a m.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> to_quaternion(const Matrix<T, 3, 3>& m)
    {
        // Shepperd's method: compute the largest of the four components from
        // the diagonal, and derive the other three from it. Using the largest
        // one keeps the division well-conditioned.
        const auto trace = m[0, 0] + m[1, 1] + m[2, 2];
        if (trace > 0)
        {
            const auto s = T(2) * std::sqrt(trace + T(1));
            return {
                s / 4,
                (m[2, 1] - m[1, 2]) / s,
                (m[0, 2] - m[2, 0]) / s,
                (m[1, 0] - m[0, 1]) / s
            };
        }

        if (m[0, 0] > m[1, 1] && m[0, 0] > m[2, 2])
        {
            const auto s = T(2) * std::sqrt(T(1) + m[0, 0] - m[1, 1] - m[2, 2]);
            return {
                (m[2, 1] - m[1, 2]) / s,
                s / 4,
                (m[0, 1] + m[1, 0]) / s,
                (m[0, 2] + m[2, 0]) / s
            };
        }

        if (m[1, 1] > m[2, 2])
        {
            const auto s = T(2) * std::sqrt(T(1) + m[1, 1] - m[0, 0] - m[2, 2]);
            return {
                (m[0, 2] - m[2, 0]) / s,
                (m[0, 1] + m[1, 0]) / s,
                s / 4,
                (m[1, 2] + m[2, 1]) / s
            };
        }

        const auto s = T(2) * std::sqrt(T(1) + m[2, 2] - m[0, 0] - m[1, 1]);
        return {
            (m[1, 0] - m[0, 1]) / s,
            (m[0, 2] + m[2, 0]) / s,
            (m[1, 2] + m[2, 1]) / s,
            s / 4
        };
    }

    /**
     * @brief Returns the unit quaternion that corresponds to the rotation
     *  part of the transformation matrix @a m.
     *
     * The translation and any perspective part of @a m are ignored.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> to_quaternion(const Matrix<T, 4, 4>& m)
    {
        return to_quaternion(Matrix<T, 3, 3>(
            m[0, 0], m[0, 1], m[0, 2],
            m[1, 0], m[1, 1], m[1, 2],
            m[2, 0], m[2, 1], m[2, 2]));
    }

    /**
     * @brief Returns the unit quaternion that rotates the x-axis onto
     *  @a longitudinal and the y-axis onto the part of @a lateral that is
     *  perpendicular to @a longitudinal.
     *
     * The z-axis ends up along the cross product of the two vectors. Neither
     * vector has to be of unit length, and @a lateral doesn't have to be
     * perpendicular to @a longitudinal, but the two must be non-zero and not
     * parallel.
     */
    template <std::floating_point T>
    [[nodiscard]]
    Quaternion<T> to_quaternion(const Vector<T, 3>& longitudinal,
                                const Vector<T, 3>& lateral)
    {
        auto z = cross(longitudinal, lateral);
        if (get_length_squared(z) == 0)
            XYZ_THROW("The vectors are zero or parallel.");
        z = normalize(z);
        const auto x = normalize(longitudinal);
        // The part of the lateral vector that is perpendicular to the
        // longitudinal one. Deriving it from z keeps the basis orthonormal.
        const auto y = cross(z, x);
        // The rotation matrix has the axis vectors as its columns.
        return to_quaternion(Matrix<T, 3, 3>(
            x[0], y[0], z[0],
            x[1], y[1], z[1],
            x[2], y[2], z[2]));
    }

    using QuaternionF = Quaternion<float>;
    using QuaternionD = Quaternion<double>;
}