//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-07-15.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Orientation.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Xyz::to_radians;
using Catch::Matchers::WithinAbs;

TEST_CASE("Vectors from 2D orientation")
{
    Xyz::Orientation2D o(to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector2D(-1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector2D(1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Vectors from 3D orientation, yaw only")
{
    Xyz::Orientation3D o(to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(1.0 / sqrt(2), -1.0 / sqrt(2), 0)));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(0, 0, 1)));
}

TEST_CASE("Vectors from 3D orientation, roll only")
{
    Xyz::Orientation3D o(0, 0, to_radians(-135.0));
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(1, 0, 0)));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(0, -1.0 / sqrt(2), -1.0 / sqrt(2))));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(0, 1.0 / sqrt(2), -1.0 / sqrt(2))));
}

TEST_CASE("Vectors from 3D orientation, pitch only")
{
    Xyz::Orientation3D o(0, to_radians(-135), 0);
    CHECK(are_equal(get_x_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), 0, 1.0 / sqrt(2))));
    CHECK(are_equal(get_y_vector(o),
        Xyz::Vector3D(0, 1, 0)));
    CHECK(are_equal(get_z_vector(o),
        Xyz::Vector3D(-1.0 / sqrt(2), 0, -1.0 / sqrt(2))));
}

TEST_CASE("Normalize 2D orientation")
{
    Xyz::Orientation2D o(to_radians(-500.0));
    auto no = normalize(o);
    CHECK_THAT(no.angle, WithinAbs(to_radians(-140.0), 1e-10));
}

TEST_CASE("Normalize 3D orientation, yaw and roll only")
{
    Xyz::Orientation3D o(to_radians(-500.0), 0, to_radians(200.0));
    auto no = normalize(o);
    CHECK_THAT(no.yaw, WithinAbs(to_radians(-140.0), 1e-10));
    CHECK_THAT(no.pitch, WithinAbs(0.0, 1e-10));
    CHECK_THAT(no.roll, WithinAbs(to_radians(-160.0), 1e-10));
}

TEST_CASE("Normalize 3D orientation, pitch only")
{
    Xyz::Orientation3D o(0, to_radians(-500.0), 0);
    auto no = normalize(o);
    CHECK_THAT(no.yaw, WithinAbs(to_radians(180.0), 1e-10));
    CHECK_THAT(no.pitch, WithinAbs(to_radians(-40.0), 1e-10));
    CHECK_THAT(no.roll, WithinAbs(to_radians(180), 1e-10));
}

TEST_CASE("Reverse 3D orientation")
{
    Xyz::Orientation3D o(to_radians(-500.0), to_radians(200.0), to_radians(100.0));
    auto ro = reverse(o);
    CHECK_THAT(ro.yaw, WithinAbs(to_radians(-320.0), 1e-10));
    CHECK_THAT(ro.pitch, WithinAbs(to_radians(20.0), 1e-10));
    CHECK_THAT(ro.roll, WithinAbs(to_radians(-80.0), 1e-10));
}

TEST_CASE("to_orientation from 3D vector")
{
    Xyz::Vector3D v(1, 2, 3);
    auto o = to_orientation(v);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(-std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(o.roll, WithinAbs(0.0, 1e-10));
}

TEST_CASE("to_orientation from 3D vectors, no roll")
{
    Xyz::Vector3D longitudinal(1, 2, 3);
    Xyz::Vector3D lateral(-1, 0.5, 0);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(std::atan2(3, sqrt(5)), 1e-10));
    CHECK_THAT(o.roll, WithinAbs(0, 1e-10));
}

TEST_CASE("to_orientation from 3D vectors, no pitch")
{
    Xyz::Vector3D longitudinal(1, 2, 0);
    Xyz::Vector3D lateral(-1, 0.5, 1);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(std::atan2(2, 1), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(o.roll, WithinAbs(std::atan2(1, std::hypot(-1, 0.5)), 1e-10));
}

TEST_CASE("to_orientation from 3D vectors, no pitch, non-orthogonal lateral")
{
    Xyz::Vector3D longitudinal(1, 1, 0);
    Xyz::Vector3D lateral(0, 4, 2);
    auto o = to_orientation(longitudinal, lateral);
    CHECK_THAT(o.yaw, WithinAbs(to_radians(45), 1e-10));
    CHECK_THAT(o.pitch, WithinAbs(0, 1e-10));
    CHECK_THAT(o.roll, WithinAbs(atan2(1, sqrt(2)), 1e-10));
}
