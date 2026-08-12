//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-12.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Quaternion.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <Xyz/Orientation.hpp>
#include <Xyz/RotationMatrix.hpp>
#include <Xyz/TransformationMatrix.hpp>

using Xyz::to_radians;
using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double MARGIN = 1e-12;

    const Xyz::Vector3D X_AXIS(1, 0, 0);
    const Xyz::Vector3D Y_AXIS(0, 1, 0);
    const Xyz::Vector3D Z_AXIS(0, 0, 1);
}

TEST_CASE("Quaternion: default is the identity rotation")
{
    constexpr Xyz::QuaternionD q;
    REQUIRE(q.w == 1);
    REQUIRE(q.v == Xyz::Vector3D());
    CHECK(are_equal(rotate(q, Xyz::Vector3D(1, 2, 3)),
                    Xyz::Vector3D(1, 2, 3), MARGIN));
}

TEST_CASE("Quaternion: axis and angle round-trip")
{
    const auto q = make_quaternion(to_radians(60.0), Xyz::Vector3D(1, 1, 1));
    CHECK_THAT(get_length(q), WithinAbs(1.0, MARGIN));
    CHECK_THAT(get_angle(q), WithinAbs(to_radians(60.0), MARGIN));
    CHECK(are_equal(get_axis(q), normalize(Xyz::Vector3D(1, 1, 1)), MARGIN));
}

TEST_CASE("Quaternion: rotate a vector around each axis")
{
    // Counterclockwise seen from the positive end of the axis.
    const auto yaw = make_quaternion(to_radians(90.0), Z_AXIS);
    CHECK(are_equal(rotate(yaw, X_AXIS), Y_AXIS, MARGIN));

    const auto pitch = make_quaternion(to_radians(90.0), Y_AXIS);
    CHECK(are_equal(rotate(pitch, Z_AXIS), X_AXIS, MARGIN));

    const auto roll = make_quaternion(to_radians(90.0), X_AXIS);
    CHECK(are_equal(rotate(roll, Y_AXIS), Z_AXIS, MARGIN));
}

TEST_CASE("Quaternion: the product applies the right hand side first")
{
    const auto yaw = make_quaternion(to_radians(90.0), Z_AXIS);
    const auto roll = make_quaternion(to_radians(90.0), X_AXIS);

    CHECK(are_equal(rotate(yaw * roll, Y_AXIS),
                    rotate(yaw, rotate(roll, Y_AXIS)), MARGIN));
    CHECK(are_equal(rotate(yaw * roll, Y_AXIS), Z_AXIS, MARGIN));
    CHECK(are_equal(rotate(roll * yaw, Y_AXIS), -X_AXIS, MARGIN));
}

TEST_CASE("Quaternion: conjugate and invert undo the rotation")
{
    const auto q = make_quaternion(to_radians(35.0), Xyz::Vector3D(1, -2, 3));
    const Xyz::Vector3D v(4, 5, 6);

    CHECK(are_equal(rotate(conjugate(q), rotate(q, v)), v, MARGIN));
    CHECK(are_equal(q * invert(q), Xyz::QuaternionD(), MARGIN));

    // invert() also handles quaternions that are not unit length.
    const auto p = q * 3.0;
    CHECK(are_equal(p * invert(p), Xyz::QuaternionD(), MARGIN));
}

TEST_CASE("Quaternion: to_matrix matches the axis rotation matrices")
{
    const auto angle = to_radians(35.0);

    CHECK(are_equal(Xyz::linear::to_matrix(make_quaternion(angle, Z_AXIS)),
                    Xyz::linear::rotate_z(angle), MARGIN));
    CHECK(are_equal(Xyz::linear::to_matrix(make_quaternion(angle, X_AXIS)),
                    Xyz::linear::rotate_x(angle), MARGIN));

    // There is no linear::rotate_y, so compare with the generic axis rotation.
    for (const auto& axis : {X_AXIS, Y_AXIS, Z_AXIS})
    {
        CAPTURE(axis);
        CHECK(are_equal(Xyz::linear::to_matrix(make_quaternion(angle, axis)),
                        Xyz::linear::rotate3(angle, axis), MARGIN));
    }
}

TEST_CASE("Quaternion: to_matrix rotates vectors the same way as rotate")
{
    const auto q = make_quaternion(to_radians(-71.0), Xyz::Vector3D(2, 3, -1));
    const Xyz::Vector3D v(4, -5, 6);
    CHECK(are_equal(Xyz::linear::to_matrix(q) * v, rotate(q, v), MARGIN));
}

TEST_CASE("Quaternion: to_matrix ignores the length of the quaternion")
{
    const auto q = make_quaternion(to_radians(23.0), Xyz::Vector3D(1, 2, 3));
    CHECK(are_equal(Xyz::linear::to_matrix(q * 4.0),
                    Xyz::linear::to_matrix(q), MARGIN));
}

TEST_CASE("Quaternion: affine to_matrix adds the translation")
{
    const auto angle = to_radians(35.0);
    const auto q = make_quaternion(angle, Z_AXIS);
    const auto m = Xyz::affine::to_matrix(q, Xyz::Vector3D(7, 8, 9));

    CHECK(are_equal(m, Xyz::affine::translate3(Xyz::Vector3D(7, 8, 9))
                       * Xyz::affine::rotate_z(angle), MARGIN));
}

TEST_CASE("Quaternion: matrix round-trip covers all four Shepperd branches")
{
    // Each of these picks a different largest component, so between them they
    // exercise every branch of to_quaternion(Matrix<T, 3, 3>).
    const Xyz::QuaternionD quaternions[] = {
        make_quaternion(to_radians(20.0), Xyz::Vector3D(1, 2, 3)),
        make_quaternion(to_radians(179.0), X_AXIS),
        make_quaternion(to_radians(179.0), Y_AXIS),
        make_quaternion(to_radians(179.0), Z_AXIS)
    };

    for (const auto& q : quaternions)
    {
        CAPTURE(q);
        const auto m = Xyz::linear::to_matrix(q);
        const auto q2 = to_quaternion(m);
        CHECK_THAT(get_length(q2), WithinAbs(1.0, 1e-9));
        CHECK(are_equivalent(q, q2, 1e-9));
        CHECK(are_equal(Xyz::linear::to_matrix(q2), m, 1e-9));
    }
}

TEST_CASE("Quaternion: round-trip through a 4x4 matrix ignores translation")
{
    const auto q = make_quaternion(to_radians(48.0), Xyz::Vector3D(-1, 2, 5));
    const auto m = Xyz::affine::to_matrix(q, Xyz::Vector3D(7, 8, 9));
    CHECK(are_equivalent(to_quaternion(m), q, 1e-9));
}

TEST_CASE("Quaternion: to_quaternion of the identity matrix")
{
    CHECK(are_equal(to_quaternion(Xyz::Matrix3D::identity()),
                    Xyz::QuaternionD(), MARGIN));
}

TEST_CASE("Orientation: to_quaternion matches to_matrix")
{
    const Xyz::Orientation3D orientations[] = {
        {},
        {to_radians(90.0), 0, 0},
        {0, to_radians(90.0), 0},
        {0, 0, to_radians(90.0)},
        {to_radians(35.0), to_radians(-20.0), to_radians(125.0)},
        {to_radians(-160.0), to_radians(70.0), to_radians(-45.0)}
    };

    for (const auto& o : orientations)
    {
        CAPTURE(o);
        const auto q = to_quaternion(o);
        CHECK_THAT(get_length(q), WithinAbs(1.0, MARGIN));
        CHECK(are_equal(Xyz::linear::to_matrix(q), Xyz::linear::to_matrix(o), 1e-9));

        // The axis vectors are the columns of the rotation matrix.
        const auto [x, y, z] = get_vectors(o);
        CHECK(are_equal(rotate(q, X_AXIS), x, 1e-9));
        CHECK(are_equal(rotate(q, Y_AXIS), y, 1e-9));
        CHECK(are_equal(rotate(q, Z_AXIS), z, 1e-9));
    }
}

TEST_CASE("Orientation: quaternion round-trip")
{
    const Xyz::Orientation3D orientations[] = {
        {},
        {to_radians(35.0), to_radians(-20.0), to_radians(125.0)},
        {to_radians(-160.0), to_radians(70.0), to_radians(-45.0)},
        {to_radians(180.0), 0, to_radians(180.0)}
    };

    for (const auto& o : orientations)
    {
        CAPTURE(o);
        const auto o2 = to_orientation(to_quaternion(o));
        CHECK_THAT(o2.yaw, WithinAbs(o.yaw, 1e-9));
        CHECK_THAT(o2.pitch, WithinAbs(o.pitch, 1e-9));
        CHECK_THAT(o2.roll, WithinAbs(o.roll, 1e-9));
    }
}

TEST_CASE("Orientation: quaternion round-trip at gimbal lock")
{
    // Pitch straight down and straight up. Yaw and roll are the same axis
    // there, so the round-trip puts the whole rotation in yaw, and only the
    // resulting rotation can be compared.
    const Xyz::Orientation3D orientations[] = {
        {to_radians(30.0), to_radians(90.0), to_radians(40.0)},
        {to_radians(30.0), to_radians(-90.0), to_radians(40.0)}
    };

    for (const auto& o : orientations)
    {
        CAPTURE(o);
        const auto o2 = to_orientation(to_quaternion(o));
        CHECK_THAT(o2.roll, WithinAbs(0.0, 1e-9));
        CHECK_THAT(std::abs(o2.pitch), WithinAbs(Xyz::Constants<double>::PI / 2, 1e-6));
        CHECK(are_equal(Xyz::linear::to_matrix(o2),
                        Xyz::linear::to_matrix(o), 1e-9));
    }
}