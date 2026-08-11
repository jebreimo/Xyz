//****************************************************************************
// Copyright © 2025 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2025-05-19.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Pgram.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Pgram: invalid parallelogram")
{
    using P = Xyz::Pgram<float, 3>;
    P rect;
    REQUIRE(!rect.is_valid());
    rect = P{{0, 0, 0}, {1, 0, 0}, {-2, 0, 0}};
    REQUIRE(!rect.is_valid());
}

TEST_CASE("Pgram: rectangular parallelogram")
{
    using P = Xyz::Pgram<float, 3>;
    REQUIRE(is_rectangle(P{{0, 0, 0}, {-3, 4, 0}, {4, 3, 0}}));
    REQUIRE(!is_rectangle(P{{0, 0, 0}, {-3, 3, 0}, {4, 3, 0}}));
}

template <typename T>
void require_is_clip_rect(const Xyz::Matrix<T, 4, 4>& m, const Xyz::Pgram<T, 3>& p)
{
    using V = Xyz::Vector<T, 4>;
    CAPTURE(p.origin);
    CAPTURE(p.edge0);
    CAPTURE(p.edge1);
    T e = Xyz::Margin<T>::DEFAULT;
    REQUIRE(are_equal(m * make_vector4(Xyz::rel_to_abs(p, {0, 0}), 1), V(0, 0, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(Xyz::rel_to_abs(p, {1, 0}), 1), V(1, 0, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(Xyz::rel_to_abs(p, {1, 1}), 1), V(1, 1, 0, 1), e));
    REQUIRE(are_equal(m * make_vector4(Xyz::rel_to_abs(p, {0, 1}), 1), V(0, 1, 0, 1), e));
}

TEST_CASE("Pgram: square_transform - translation")
{
    constexpr Xyz::Pgram<double, 3> p{{1, 2, 3}, {1, 0, 0}, {0, 1, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: clip_transform - rotation xy")
{
    auto d = 1 / sqrt(2);
    const Xyz::Pgram<double, 3> p{{1, 2, 3}, {d, d, 0}, {-d, d, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: clip_transform - rotation xz")
{
    auto d = 1 / sqrt(2);
    const Xyz::Pgram<double, 3> p{{1, 2, 3}, {d, 0, d}, {-d, 0, d}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: clip_transform - shearing")
{
    auto d = 1.f / std::sqrt(2.f);
    const Xyz::Pgram<float, 3> p{{1, 2, 3}, {1, 0, 0}, {1, d, d}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: clip_transform - scale")
{
    constexpr Xyz::Pgram<double, 3> p{{1, 2, 3}, {5, 0, 0}, {0, 3, 0}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: clip_transform - float")
{
    constexpr Xyz::Pgram<float, 3> p{{1, 2, 3}, {5, 3, 2}, {1, 4, 8}};
    const auto m = get_clip_transform(p);
    require_is_clip_rect(m, p);
}

TEST_CASE("Pgram: bounding box")
{
    using P = Xyz::Pgram<float, 3>;
    constexpr P pg{{0, 0, 0}, {4, 0, 0}, {2, 2, 0}};
    auto rect = get_bounding_rect(pg);
    REQUIRE(rect.placement.origin == Xyz::Vector3F(0, 0, 0));
    REQUIRE(rect.size.x() == 6);
    REQUIRE(rect.size.y() == 2);
}

TEST_CASE("Pgram: length and width")
{
    using P = Xyz::Pgram<float, 3>;
    constexpr P pg{{0, 0, 0}, {4, 1, 0}, {1, 4, 0}};
    REQUIRE_THAT(pg.length(), WithinAbs((1.f + 8.f / 17.f) * sqrt(17.f), 1e-6));
    REQUIRE_THAT(pg.width(), WithinAbs(sqrt(17.f - 64.f / 17.f), 1e-6));
}
