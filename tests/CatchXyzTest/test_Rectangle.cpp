//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-04-27.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Rectangle.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Rectangle: equality")
{
    Xyz::RectangleI a({1, 2}, {10, 20});
    Xyz::RectangleI b({1, 2}, {10, 20});
    Xyz::RectangleI c({1, 2}, {10, 21});
    Xyz::RectangleI d({2, 2}, {10, 20});
    REQUIRE(a == b);
    REQUIRE(a != c);
    REQUIRE(a != d);
}

TEST_CASE("Rectangle: is_empty and is_null")
{
    REQUIRE(!is_empty(Xyz::RectangleI({0, 0}, {5, 5})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {0, 5})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {5, 0})));
    REQUIRE(is_empty(Xyz::RectangleI({0, 0}, {0, 0})));

    REQUIRE(!is_null(Xyz::RectangleI({0, 0}, {5, 5})));
    REQUIRE(!is_null(Xyz::RectangleI({0, 0}, {0, 5})));
    REQUIRE(is_null(Xyz::RectangleI({0, 0}, {0, 0})));
}

TEST_CASE("Rectangle: get_min and get_max")
{
    Xyz::RectangleI pos({2, 3}, {10, 20});
    REQUIRE(get_min(pos) == Xyz::Vector2I(2, 3));
    REQUIRE(get_max(pos) == Xyz::Vector2I(12, 23));

    Xyz::RectangleI neg({2, 3}, {-10, -20});
    REQUIRE(get_min(neg) == Xyz::Vector2I(-8, -17));
    REQUIRE(get_max(neg) == Xyz::Vector2I(2, 3));
}

TEST_CASE("Rectangle: get_center and set_center")
{
    Xyz::RectangleI rect({0, 0}, {10, 20});
    REQUIRE(get_center(rect) == Xyz::Vector2I(5, 10));

    set_center(rect, Xyz::Vector2I(0, 0));
    REQUIRE(rect.origin == Xyz::Vector2I(-5, -10));
    REQUIRE(get_center(rect) == Xyz::Vector2I(0, 0));
}

TEST_CASE("Rectangle: normalize")
{
    using R = Xyz::RectangleI;
    REQUIRE(normalize(R({2, 3}, {10, 20})) == R({2, 3}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {-10, 20})) == R({-8, 3}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {10, -20})) == R({2, -17}, {10, 20}));
    REQUIRE(normalize(R({2, 3}, {-10, -20})) == R({-8, -17}, {10, 20}));
}

TEST_CASE("Rectangle: indexed access")
{
    using V = Xyz::Vector2I;
    const Xyz::RectangleI rect({2, 3}, {5, 7});
    REQUIRE(rect[0] == V(2, 3));
    REQUIRE(rect[1] == V(7, 3));
    REQUIRE(rect[2] == V(7, 10));
    REQUIRE(rect[3] == V(2, 10));
    REQUIRE(rect[4] == V(2, 3));
}

TEST_CASE("Rectangle: get_union (int)")
{
    using R = Xyz::RectangleI;

    // Non-overlapping, side by side
    REQUIRE(get_union(R({0, 0}, {5, 5}), R({10, 0}, {5, 5})) == R({0, 0}, {15, 5}));

    // Overlapping
    REQUIRE(get_union(R({0, 0}, {10, 10}), R({5, 5}, {10, 10})) == R({0, 0}, {15, 15}));

    // One contains the other
    REQUIRE(get_union(R({0, 0}, {10, 10}), R({2, 2}, {5, 5})) == R({0, 0}, {10, 10}));

    // Identical rectangles
    REQUIRE(get_union(R({1, 2}, {10, 0}), R({1, 2}, {0, 20})) == R({1, 2}, {10, 20}));

    // Rectangles with negative sizes (get_min/get_max normalise them)
    // R({10, 10}, {-5, -5}): min=(5,5), max=(10,10)
    // R({0, 0}, {3, 3}):     min=(0,0), max=(3,3)
    // union: origin=(0,0), size=(10,10)
    REQUIRE(get_union(R({10, 10}, {-15, -15}), R({8, 8}, {3, 3})) == R({-5, -5}, {16, 16}));
}

TEST_CASE("Rectangle: get_union (float)")
{
    using R = Xyz::RectangleF;

    // Non-overlapping
    REQUIRE(get_union(R({0.f, 0.f}, {1.5f, 2.f}), R({3.f, 0.f}, {1.f, 2.f}))
            == R({0.f, 0.f}, {4.f, 2.f}));

    // Overlapping
    REQUIRE(get_union(R({0.f, 0.f}, {2.f, 2.f}), R({1.f, 1.f}, {2.f, 2.f}))
            == R({0.f, 0.f}, {3.f, 3.f}));

    // One contains the other
    REQUIRE(get_union(R({0.f, 0.f}, {10.f, 10.f}), R({2.5f, 2.5f}, {4.f, 4.f}))
            == R({0.f, 0.f}, {10.f, 10.f}));
}
