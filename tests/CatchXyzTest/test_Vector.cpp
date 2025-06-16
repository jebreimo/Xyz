//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-12-25.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/SphericalPoint.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

constexpr auto PI = Xyz::Constants<double>::PI;
constexpr auto MARGIN = Xyz::Margin<double>::default_margin();

using Catch::Matchers::WithinAbs;

void test_from_xyz_to_spherical_and_back(Xyz::Vector3D cartesian,
                                         Xyz::SphericalPoint<double> spherical)
{
    auto sp = Xyz::to_spherical<double>(cartesian);
    REQUIRE_THAT(sp.radius, WithinAbs(spherical.radius, MARGIN));
    REQUIRE_THAT(sp.azimuth, WithinAbs(spherical.azimuth, MARGIN));
    REQUIRE_THAT(sp.polar, WithinAbs(spherical.polar, MARGIN));
    auto p = Xyz::to_cartesian(sp);
    REQUIRE_THAT(p[0], WithinAbs(cartesian[0], MARGIN));
    REQUIRE_THAT(p[1], WithinAbs(cartesian[1], MARGIN));
    REQUIRE_THAT(p[2], WithinAbs(cartesian[2], MARGIN));
}

TEST_CASE("xyz to spherical")
{
    SECTION("0°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, 0, -1},
            {sqrt(2), 0, -PI / 4});
    }
    SECTION("45°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, 1, 1},
            {sqrt(3), PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("90°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {0, 1, -1},
            {sqrt(2), PI / 2, -PI / 4});
    }
    SECTION("135°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 1, 1},
            {sqrt(3), 3 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("180°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 0, -1},
            {sqrt(2), PI, -PI / 4});
    }
    SECTION("-135°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, -1, 1},
            {sqrt(3), -3 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("180°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {-1, 0, -1},
            {sqrt(2), PI, -PI / 4});
    }
    SECTION("-45°, 35.3°")
    {
        test_from_xyz_to_spherical_and_back(
            {1, -1, 1},
            {sqrt(3), -1 * PI / 4, asin(1 / sqrt(3))});
    }
    SECTION("-90°, -45°")
    {
        test_from_xyz_to_spherical_and_back(
            {0, -1, -1},
            {sqrt(2), -PI / 2, -PI / 4});
    }
}

TEST_CASE("clamp vector")
{
    Xyz::Vector4D v1(1, -2, 3, -4);
    auto v2 = get_clamped(v1, -1, 1);
    REQUIRE(v2 == Xyz::Vector4D(1, -1, 1, -1));
    REQUIRE(v1 == Xyz::Vector4D(1, -2, 3, -4));
    clamp_inplace(v1, -1, 1);
    REQUIRE(v1 == Xyz::Vector4D(1, -1, 1, -1));
}

TEST_CASE("clamp vector with min and max vectors")
{
    using Vec = Xyz::Vector2D;
    constexpr Vec min(-2, 3);
    constexpr Vec max(8, 13);
    REQUIRE(get_clamped(Vec(-1, 4), min, max) == Vec(-1, 4));
    REQUIRE(get_clamped(Vec(-3, 4), min, max) == Vec(-2, 4));
    REQUIRE(get_clamped(Vec(9, 4), min, max) == Vec(8, 4));
    REQUIRE(get_clamped(Vec(8, 0), min, max) == Vec(8, 3));
    REQUIRE(get_clamped(Vec(-2, 14), min, max) == Vec(-2, 13));
}

TEST_CASE("scale vector")
{
    Xyz::Vector4D v1(1, -2, 3, -4);
    const auto v2 = get_scaled(v1, 1);
    const auto root = std::sqrt(1 + 2 * 2 + 3 * 3 + 4 * 4);
    REQUIRE(are_equal(v2, Xyz::Vector4D(1 / root, -2 / root, 3 / root, -4 / root)));

    scale_inplace(v1, 1);
    REQUIRE(are_equal(v1, Xyz::Vector4D(1 / root, -2 / root, 3 / root, -4 / root)));
}

TEST_CASE("vector floor")
{
    constexpr Xyz::Vector4D v(1.2, -2.2, 3.2, -4.9);
    REQUIRE(floor(v) == Xyz::Vector4D(1, -3, 3, -5));
}

TEST_CASE("vector ceil")
{
    constexpr Xyz::Vector4D v(1.2, -2.2, 3.2, -4.9);
    REQUIRE(ceil(v) == Xyz::Vector4D(2, -2, 4, -4));
}

TEST_CASE("Vector cross product")
{
    const auto u = Xyz::make_vector3(1, 2, 3);
    const auto v = Xyz::make_vector3(0, 1, 2);
    CHECK(cross(u, v) == Xyz::make_vector3(1, -2, 1));
}

TEST_CASE("Vector get_cos_angle")
{
    using V = Xyz::Vector2D;
    CHECK_THAT(get_cos_angle(V(3, 0), V(5, 5)), WithinAbs(1.0 / sqrt(2), 1e-10));
}

TEST_CASE("Vector get_angle")
{
    using V = Xyz::Vector3D;
    CHECK_THAT(get_angle(V(1, 0, 0), V(0, 1, 0)),
               WithinAbs(PI / 2, 1e-10));
    CHECK_THAT(get_angle(V(1, 0, 0), V(0, -1, 0)),
               WithinAbs(PI / 2, 1e-10));
    CHECK_THAT(get_angle(V(1, 0, 0), V(-1, -1, 0)),
               WithinAbs(3 * PI / 4, 1e-10));
    CHECK_THAT(get_angle(V(1, 0, 0), V(-1, 1, 0)),
               WithinAbs(3 * PI / 4, 1e-10));
}

TEST_CASE("Vector get_ccw_angle 2D")
{
    using V = Xyz::Vector2I;
    CHECK_THAT(get_ccw_angle(V(6, 6), V(4, -4)),
               WithinAbs(3 * PI / 2, 1e-10));
}

TEST_CASE("Vector get_cw_angle 3D")
{
    using V = Xyz::Vector3D;
    auto q = sqrt(8);
    CHECK_THAT(get_ccw_angle(V(1, 0, 0), V(4, q, q), V(0, -1, 1)),
               WithinAbs(PI / 4, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(0, 4, 4), V(0, -1, 1)),
               WithinAbs(PI / 2, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(-4, q, q), V(0, -1, 1)),
               WithinAbs(3 * PI / 4, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(-6, 0, 0), V(0, -1, 1)),
               WithinAbs(PI, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(-4, -q, -q), V(0, -1, 1)),
               WithinAbs(5 * PI / 4, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(0, -4, -4), V(0, -1, 1)),
               WithinAbs(3 * PI / 2, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(4, -q, -q), V(0, -1, 1)),
               WithinAbs(7 * PI / 4, 1e-10));
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(1, 0, 0), V(0, -1, 1)),
               WithinAbs(0, 1e-10));
}

TEST_CASE("get_ccw_angle 3D only considers the plane defined by the normal")
{
    using V = Xyz::Vector3D;
    auto q = sqrt(8);
    CHECK_THAT(get_ccw_angle(V(6, 0, 0), V(2, 0, 2), V(0, 0, 1)),
                WithinAbs(0, 1e-10));
}

TEST_CASE("Test vector types")
{
    auto u = Xyz::make_vector2(1, 2);
    CHECK(typeid(typename decltype(u)::ValueType).name() == typeid(int).name());
    auto v = Xyz::make_vector2(2.0, 4.0);
    CHECK(typeid(typename decltype(v)::ValueType).name() == typeid(double).name());
    auto w = vector_cast<double>(u) + v;
    CHECK(w == Xyz::Vector2D(3.0, 6.0));
    CHECK(typeid(typename decltype(w)::ValueType).name() == typeid(double).name());
}

TEST_CASE("Test vector constructors")
{
    const auto u = Xyz::make_vector4(1, 2, 0, 1);
    Xyz::Vector<double, 4> v = vector_cast<double>(u);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 0);
    CHECK(v[3] == 1);
}

TEST_CASE("Test Vector2D basics")
{
    auto u = Xyz::Vector2D(2.0, 1.0);
    auto v = Xyz::Vector2D(3.0, 4.0);
    CHECK(u[0] == 2);
    CHECK(u[1] == 1);
    CHECK(v[0] == 3);
    CHECK(v[1] == 4);
    CHECK(u * v == Xyz::Vector2D(6, 4));
    CHECK(u + v == Xyz::Vector2D(5, 5));
    CHECK(u - v == Xyz::Vector2D(-1, -3));
    CHECK(u * 3. == Xyz::Vector2D(6, 3));
    CHECK(3. * u == Xyz::Vector2D(6, 3));
    CHECK(u / 3. == Xyz::Vector2D(2.0 / 3, 1.0 / 3));
    CHECK(6. / u == Xyz::Vector2D(3, 6));
    CHECK((u += v) == Xyz::Vector2D(5, 5));
    CHECK((u -= v) == Xyz::Vector2D(2, 1));
    CHECK((u *= 3.) == Xyz::Vector2D(6, 3));
    CHECK((u /= 2.) == Xyz::Vector2D(3.0, 1.5));
    CHECK((u *= v) == Xyz::Vector2D(9, 6));
    CHECK((u *= 2.) == Xyz::Vector2D(18, 12));
    CHECK((u /= v) == Xyz::Vector2D(6, 3));
    CHECK(dot(u, v) == 30);
    CHECK_THAT(get_length(v), WithinAbs(5, 1e-10));
    std::pair values{1, 2};
    CHECK(Xyz::Vector2F(values) == Xyz::Vector2D(1.0f, 2.0f));
}

TEST_CASE("Test Vector4D basics")
{
    auto u = Xyz::Vector4D(2.0, 1.0, 4.0, 3.0);
    auto v = Xyz::Vector4D(3.0, 4.0, -1.0, -2.0);
    CHECK(u[0] == 2);
    CHECK(u[1] == 1);
    CHECK(u[2] == 4);
    CHECK(u[3] == 3);
    CHECK(v[0] == 3);
    CHECK(v[1] == 4);
    CHECK(v[2] == -1);
    CHECK(v[3] == -2);
    CHECK(u * v == Xyz::Vector4D(6, 4, -4, -6));
    CHECK(u + v == Xyz::Vector4D(5, 5, 3, 1));
    CHECK(u - v == Xyz::Vector4D(-1, -3, 5, 5));
    CHECK(u * 3. == Xyz::Vector4D(6, 3, 12, 9));
    CHECK(3. * u == Xyz::Vector4D(6, 3, 12, 9));
    CHECK(u / 3. == Xyz::Vector4D(2.0 / 3, 1.0 / 3, 4.0 / 3, 1.0));
    CHECK(12. / u == Xyz::Vector4D(6, 12, 3, 4));
    CHECK((u += v) == Xyz::Vector4D(5, 5, 3, 1));
    CHECK((u -= v) == Xyz::Vector4D(2, 1, 4, 3));
    CHECK((u *= 3.) == Xyz::Vector4D(6, 3, 12, 9));
    CHECK((u /= 2.) == Xyz::Vector4D(3.0, 1.5, 6.0, 4.5));
    CHECK((u *= v) == Xyz::Vector4D(9, 6, -6, -9));
    CHECK((u *= 2.) == Xyz::Vector4D(18, 12, -12, -18));
    CHECK((u /= v) == Xyz::Vector4D(6, 3, 12, 9));
    CHECK(dot(u, v) == 18 + 12 - 12 - 18);
    CHECK_THAT(get_length(v), WithinAbs(sqrt(9 + 16 + 1 + 4), 1e-10));
}

TEST_CASE("Test rotate Vector2D")
{
    using V = Xyz::Vector2D;
    auto sqrt2 = std::sqrt(2);
    CHECK(are_equal(rotate(V(100, 0), Xyz::to_radians(30)),
        V(50 * std::sqrt(3), 50.0), 1e-10));
    CHECK(are_equal(rotate(V(100, 0), Xyz::to_radians(45)),
        V(100 / sqrt2, 100 / sqrt2), 1e-10));
    CHECK(are_equal(rotate(V(100, 0), Xyz::to_radians(60)),
        V(50.0, 50 * std::sqrt(3)), 1e-10));
    CHECK(are_equal(rotate(V(0, 100), Xyz::to_radians(-60)),
        V(50 * std::sqrt(3), 50.0), 1e-10));
    CHECK(are_equal(rotate(V(0, 100), Xyz::to_radians(-45)),
        V(100 / sqrt2, 100 / sqrt2), 1e-10));
    CHECK(are_equal(rotate(V(0, 100), Xyz::to_radians(-30)),
        V(50.0, 50 * std::sqrt(3)), 1e-10));

    CHECK(are_equal(rotate(V(1 / sqrt2, 1 / sqrt2), Xyz::to_radians(45)),
        V(0, 1), 1e-10));
    CHECK(are_equal(rotate(V(1 / sqrt2, 1 / sqrt2), Xyz::to_radians(135)),
        V(-1, 0), 1e-10));
}
