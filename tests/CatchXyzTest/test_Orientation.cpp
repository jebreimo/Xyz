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
    Xyz::Orientation2D o(to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector2D(-1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector2D(1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Vectors from 3D orientation, yaw only")
{
    Xyz::Orientation3D o(to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(0, 0, 1)));
}

TEST_CASE("Orientation: Vectors from 3D orientation, roll only")
{
    Xyz::Orientation3D o(0, 0, to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(1, 0, 0)));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(0, -1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(0, 1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Vectors from 3D orientation, pitch only")
{
    Xyz::Orientation3D o(0, to_radians(-135), 0);
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), 0, 1.0 / sqrt(2))));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(0, 1, 0)));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), 0, -1.0 / sqrt(2))));
}

TEST_CASE("Orientation: Normalize 2D orientation")
{
    Xyz::Orientation2D o(to_radians(-500.0));
    auto no = normalize(o);
    CHECK_THAT(no.angle, WithinAbs(to_radians(-140.0), 1e-10));
}

TEST_CASE("Orientation: Normalize 3D orientation, yaw and roll only")
{
    Xyz::Orientation3D o(to_radians(-500.0), 0, to_radians(200.0));
    auto no = normalize(o);
    CHECK_THAT(no.yaw, WithinAbs(to_radians(-140.0), 1e-10));
    CHECK_THAT(no.pitch, WithinAbs(0.0, 1e-10));
    CHECK_THAT(no.roll, WithinAbs(to_radians(-160.0), 1e-10));
}

TEST_CASE("Orientation: Normalize 3D orientation, pitch only")
{
    Xyz::Orientation3D o(0, to_radians(-500.0), 0);
    auto no = normalize(o);
    CHECK_THAT(no.yaw, WithinAbs(to_radians(180.0), 1e-10));
    CHECK_THAT(no.pitch, WithinAbs(to_radians(-40.0), 1e-10));
    CHECK_THAT(no.roll, WithinAbs(to_radians(180), 1e-10));
}

TEST_CASE("Orientation: Reverse 3D orientation")
{
    Xyz::Orientation3D o(to_radians(-500.0), to_radians(200.0), to_radians(100.0));
    auto ro = reverse(o);
    CHECK_THAT(ro.yaw, WithinAbs(to_radians(-320.0), 1e-10));
    CHECK_THAT(ro.pitch, WithinAbs(to_radians(20.0), 1e-10));
    CHECK_THAT(ro.roll, WithinAbs(to_radians(-80.0), 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vector")
{
    Xyz::Vector3D v(1, 2, 3);
    auto o = to_orientation(v);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(-std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(o.roll, WithinAbs(0.0, 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no roll")
{
    Xyz::Vector3D longitudinal(1, 2, 3);
    Xyz::Vector3D lateral(-1, 0.5, 0);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(o.roll, WithinAbs(0, 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no pitch")
{
    Xyz::Vector3D longitudinal(1, 2, 0);
    Xyz::Vector3D lateral(-1, 0.5, 1);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(o.roll, WithinAbs(std::atan2(1, std::hypot(-1, 0.5)), 1e-10));
}

TEST_CASE("Orientation: to_orientation from 3D vectors, no pitch, non-orthogonal lateral")
{
    Xyz::Vector3D longitudinal(1, 1, 0);
    Xyz::Vector3D lateral(0, 4, 2);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(to_radians(45), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(o.roll, WithinAbs(atan2(1, sqrt(2)), 1e-10));
}

TEST_CASE("Orientation: 2D linear to_matrix has the axes as its columns")
{
    Xyz::Orientation2D o(to_radians(-135.0));
    const auto m = Xyz::linear::to_matrix(o);
    const auto [x, y] = get_vectors(o);

    CHECK(are_equal(m * Xyz::Vector2D(1, 0), x, 1e-10));
    CHECK(are_equal(m * Xyz::Vector2D(0, 1), y, 1e-10));
    CHECK(are_equal(m, Xyz::linear::rotate2(o.angle), 1e-10));
}

TEST_CASE("Orientation: 2D affine to_matrix adds the translation")
{
    const auto angle = to_radians(35.0);
    Xyz::Orientation2D o(angle);
    const Xyz::Vector2D offset(7, 8);

    CHECK(are_equal(Xyz::affine::to_matrix(o, offset),
                    Xyz::affine::translate2(offset)
                    * Xyz::affine::rotate2(angle), 1e-10));
}

TEST_CASE("Orientation: 2D affine to_matrix defaults to no translation")
{
    Xyz::Orientation2D o(to_radians(35.0));
    CHECK(are_equal(Xyz::affine::to_matrix(o),
                    Xyz::affine::rotate2(o.angle), 1e-10));
}
