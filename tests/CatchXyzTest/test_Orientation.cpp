//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-15.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Orientation.hpp>
#include <Xyz/TransformationMatrix.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Xyz::to_radians;
using Catch::Matchers::WithinAbs;

TEST_CASE("Orientation: Vectors from 2D orientation")
{
    constexpr Xyz::Orientation2D o(to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector2D(-1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector2D(1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Vectors from 3D orientation, yaw only")
{
    constexpr Xyz::Orientation3D o(to_radians(-135.0));
    const auto [x, y, z] = get_vectors(o);
    CHECK(are_equal(x,
        Xyz::Vector3D(-1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(y,
        Xyz::Vector3D(1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(z,
        Xyz::Vector3D(0, 0, 1)));
}

TEST_CASE("Orientation: Vectors from 3D orientation, roll only")
{
    constexpr Xyz::Orientation3D o(0, 0, to_radians(-135.0));
    const auto [x, y, z] = get_vectors(o);
    CHECK(are_equal(x,
        Xyz::Vector3D(1, 0, 0)));
    CHECK(are_equal(y,
        Xyz::Vector3D(0, -1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(z,
        Xyz::Vector3D(0, 1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Vectors from 3D orientation, pitch only")
{
    constexpr Xyz::Orientation3D o(0, to_radians(-135), 0);
    const auto [x, y, z] = get_vectors(o);
    CHECK(are_equal(x,
        Xyz::Vector3D(-1.0 / sqrt(2), 0, 1.0 / sqrt(2))));
    CHECK(are_equal(y,
        Xyz::Vector3D(0, 1, 0)));
    CHECK(are_equal(z,
        Xyz::Vector3D(-1.0 / sqrt(2), 0, -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Normalize 2D orientation")
{
    constexpr Xyz::Orientation2D o(to_radians(-500.0));
    auto no = normalize(o);
    CHECK_THAT(no.angle, WithinAbs(to_radians(-140.0), 1e-10));
}

TEST_CASE("Orientation: Normalize 3D orientation, yaw and roll only")
{
    constexpr Xyz::Orientation3D o(to_radians(-500.0), 0, to_radians(200.0));
    const auto [yaw, pitch, roll] = normalize(o);
    CHECK_THAT(yaw, WithinAbs(to_radians(-140.0), 1e-10));
    CHECK_THAT(pitch, WithinAbs(0.0, 1e-10));
    CHECK_THAT(roll, WithinAbs(to_radians(-160.0), 1e-10));
}

TEST_CASE("Orientation: Normalize 3D orientation, pitch only")
{
    constexpr Xyz::Orientation3D o(0, to_radians(-500.0), 0);
    const auto [yaw, pitch, roll] = normalize(o);
    CHECK_THAT(yaw, WithinAbs(to_radians(180.0), 1e-10));
    CHECK_THAT(pitch, WithinAbs(to_radians(-40.0), 1e-10));
    CHECK_THAT(roll, WithinAbs(to_radians(180), 1e-10));
}

TEST_CASE("Orientation: Reverse 3D orientation")
{
    constexpr Xyz::Orientation3D o(to_radians(-500.0), to_radians(200.0), to_radians(100.0));
    const auto [yaw, pitch, roll] = reverse(o);
    CHECK_THAT(yaw, WithinAbs(to_radians(-320.0), 1e-10));
    CHECK_THAT(pitch, WithinAbs(to_radians(20.0), 1e-10));
    CHECK_THAT(roll, WithinAbs(to_radians(-80.0), 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vector")
{
    constexpr Xyz::Vector3D v(1, 2, 3);
    const auto [yaw, pitch, roll] = to_orientation(v);
    CHECK_THAT(yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(pitch, WithinAbs(-std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(roll, WithinAbs(0.0, 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no roll")
{
    constexpr Xyz::Vector3D longitudinal(1, 2, 3);
    constexpr Xyz::Vector3D lateral(-1, 0.5, 0);
    const auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(-std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(o.roll, WithinAbs(0, 1e-10));

    // The orientation's axis vectors are the vectors it was made from.
    const auto [x, y, z] = get_vectors(o);
    CHECK(are_equal(x, normalize(longitudinal), 1e-10));
    CHECK(are_equal(y, normalize(lateral), 1e-10));
    CHECK(are_equal(z, normalize(cross(longitudinal, lateral)), 1e-10));
}

TEST_CASE("Orientation: to_orientation ignores the lateral vector's longitudinal part")
{
    // Adding a multiple of the longitudinal vector to the lateral one leaves
    // the frame they define unchanged, and must leave the orientation
    // unchanged too, pitch or no pitch.
    constexpr Xyz::Vector3D longitudinal(1, 2, 3);
    constexpr Xyz::Vector3D lateral(-1, 0.5, 0);
    const auto [yaw, pitch, roll] = to_orientation(longitudinal, lateral);
    for (const auto factor : {-2.0, -0.3, 0.7, 5.0})
    {
        CAPTURE(factor);
        const auto [yaw2, pitch2, roll2] =
            to_orientation(longitudinal, lateral + factor * longitudinal);
        CHECK_THAT(yaw2, WithinAbs(yaw, 1e-10));
        CHECK_THAT(pitch2, WithinAbs(pitch, 1e-10));
        CHECK_THAT(roll2, WithinAbs(roll, 1e-10));
    }
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no pitch")
{
    constexpr Xyz::Vector3D longitudinal(1, 2, 0);
    constexpr Xyz::Vector3D lateral(-1, 0.5, 1);
    const auto [yaw, pitch, roll] = to_orientation(longitudinal, lateral);
    CHECK_THAT(yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(roll, WithinAbs(std::atan2(1, std::hypot(-1, 0.5)), 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no pitch, non-orthogonal lateral")
{
    constexpr Xyz::Vector3D longitudinal(1, 1, 0);
    constexpr Xyz::Vector3D lateral(0, 4, 2);
    const auto [yaw, pitch, roll] = to_orientation(longitudinal, lateral);
    CHECK_THAT(yaw, WithinAbs(to_radians(45), 1e-10));
    CHECK_THAT(pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(roll, WithinAbs(atan2(1, sqrt(2)), 1e-10));
}

TEST_CASE("Orientation: 2D linear to_matrix has the axes as its columns")
{
    constexpr Xyz::Orientation2D o(to_radians(-135.0));
    const auto m = Xyz::linear::to_matrix(o);
    const auto [x, y] = get_vectors(o);

    CHECK(are_equal(m * Xyz::Vector2D(1, 0), x, 1e-10));
    CHECK(are_equal(m * Xyz::Vector2D(0, 1), y, 1e-10));
    CHECK(are_equal(m, Xyz::linear::rotate2(o.angle), 1e-10));
}

TEST_CASE("Orientation: 2D affine to_matrix adds the translation")
{
    constexpr auto angle = to_radians(35.0);
    constexpr Xyz::Orientation2D o(angle);
    constexpr Xyz::Vector2D offset(7, 8);

    CHECK(are_equal(Xyz::affine::to_matrix(o, offset),
        Xyz::affine::translate2(offset)
        * Xyz::affine::rotate2(angle), 1e-10));
}

TEST_CASE("Orientation: 2D affine to_matrix defaults to no translation")
{
    constexpr Xyz::Orientation2D o(to_radians(35.0));
    CHECK(are_equal(Xyz::affine::to_matrix(o),
        Xyz::affine::rotate2(o.angle), 1e-10));
}

TEST_CASE("Orientation: 3D to other axis systems")
{
    using Xyz::AxisSystem;
    using Xyz::Axis;
    using V = Xyz::Vector3D;
    using std::sqrt;

    const double sq1 = 0.5, sq2 = sqrt(2) / 2, sq3 = sqrt(3) / 2;
    constexpr Xyz::Orientation3D o(to_radians(30.0), to_radians(45.0), to_radians(60.0));
    // const auto [x, y, z] = get_vectors(o, {-Axis::Z, Axis::X, Axis::Y});

    SECTION("Default axis system")
    {
        const auto [x, y, z] = get_vectors(o);
        CHECK(are_equal(x, V{sq2 * sq3, sq1 * sq2, -sq2}, 1e-10));
        CHECK(are_equal(y, V{sq3 * sq2 * sq3 - sq1 * sq1, sq1 * sq2 * sq3 + sq1 * sq3, sq2 * sq3}, 1e-10));
        CHECK(are_equal(z, V{sq3 * sq2 * sq1 + sq1 * sq3, sq1 * sq2 * sq1 - sq3 * sq3, sq2 * sq1}, 1e-10));
    }

    SECTION(("-Z, X, Y axis system"))
    {
        const auto [x, y, z] = get_vectors(o, {-Axis::Z, Axis::X, Axis::Y});
        CHECK(are_equal(x, V{sq1 * sq2, -sq2, -sq2 * sq3}, 1e-10));
        CHECK(are_equal(y, V{sq1 * sq2 * sq3 + sq1 * sq3, sq2 * sq3, -sq3 * sq2 * sq3 + sq1 * sq1}, 1e-10));
        CHECK(are_equal(z, V{sq1 * sq2 * sq1 - sq3 * sq3, sq2 * sq1, -sq3 * sq2 * sq1 - sq1 * sq3}, 1e-10));
    }
}
