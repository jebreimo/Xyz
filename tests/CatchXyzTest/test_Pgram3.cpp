//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-19.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Pgram3.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Pgram3: invalid parallelogram")
{
    using P = Xyz::Pgram3<float>;
    P rect;
    REQUIRE(!rect.is_valid());
    rect = P{{0, 0, 0}, {1, 0, 0}, {-2, 0, 0}};
    REQUIRE(!rect.is_valid());
}

TEST_CASE("Pgram3: rectangular parallelogram")
{
    using P = Xyz::Pgram3<float>;
    REQUIRE(is_rectangle(P{{0, 0, 0}, {-3, 4, 0}, {4, 3, 0}}));
    REQUIRE(!is_rectangle(P{{0, 0, 0}, {-3, 3, 0}, {4, 3, 0}}));
}

template <typename T>
void require_is_clip_rect(const Xyz::Matrix<T, 4, 4>& m, const Xyz::Pgram3<T>& p)
{
    using V = Xyz::Vector<T, 4>;
    CAPTURE(p.origin);
    CAPTURE(p.edge0);
    CAPTURE(p.edge1);
    T e = Xyz::Margin<T>::DEFAULT;
    REQUIRE(are_equal(m * make_vector4(p[0], 1), V(0, 0, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(p[1], 1), V(1, 0, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(p[2], 1), V(1, 1, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(p[3], 1), V(0, 1, 0, 1), e));
}

TEST_CASE("Pgram3: square_transform - translation")
{
    constexpr Xyz::Pgram3<double> p{{1, 2, 3}, {1, 0, 0}, {0, 1, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: clip_transform - rotation xy")
{
    auto d = 1 / sqrt(2);
    const Xyz::Pgram3<double> p{{1, 2, 3}, {d, d, 0}, {-d, d, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: clip_transform - rotation xz")
{
    auto d = 1 / sqrt(2);
    const Xyz::Pgram3<double> p{{1, 2, 3}, {d, 0, d}, {-d, 0, d}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: clip_transform - shearing")
{
    auto d = 1.f / sqrt(2.f);
    const Xyz::Pgram3<float> p{{1, 2, 3}, {1, 0, 0}, {1, d, d}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: clip_transform - scale")
{
    constexpr Xyz::Pgram3<double> p{{1, 2, 3}, {5, 0, 0}, {0, 3, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: clip_transform - float")
{
    constexpr Xyz::Pgram3<float> p{{1, 2, 3}, {5, 3, 2}, {1, 4, 8}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram3: bounding box")
{
    using P = Xyz::Pgram3<float>;
    constexpr P pg{{0, 0, 0}, {4, 0, 0}, {2, 2, 0}};
    auto rect = get_bounding_rect(pg);
    REQUIRE(rect.placement.origin == Xyz::Vector3F(0, 0, 0));
    REQUIRE(rect.size.x() == 6);
    REQUIRE(rect.size.y() == 2);
}

TEST_CASE("Pgram3: length and width")
{
    using P = Xyz::Pgram3<float>;
    constexpr P pg{{0, 0, 0}, {4, 1, 0}, {1, 4, 0}};
    REQUIRE_THAT(pg.length(), WithinAbs((1.f + 8.f / 17.f) * sqrt(17.f), 1e-6));
    REQUIRE_THAT(pg.width(), WithinAbs(sqrt(17.f - 64.f / 17.f), 1e-6));
}
