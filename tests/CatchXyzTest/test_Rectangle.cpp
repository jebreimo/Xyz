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

TEST_CASE("Rectangle: rel_to_abs")
{
    using V = Xyz::Vector2I;
    const Xyz::RectangleI rect({2, 3}, {5, 7});
    REQUIRE(rel_to_abs(rect, V(0, 0)) == V(2, 3));
    REQUIRE(rel_to_abs(rect, V(1, 0)) == V(7, 3));
    REQUIRE(rel_to_abs(rect, V(1, 1)) == V(7, 10));
    REQUIRE(rel_to_abs(rect, V(0, 1)) == V(2, 10));
    REQUIRE(rel_to_abs(rect, V(2, 2)) == V(12, 17));
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

TEST_CASE("Rectangle: get_subrect")
{
    using R = Xyz::RectangleF;
    const R rect({2.f, 0.f}, {10.f, -10.f});
    const R rel_subrect({0.2f, 0.3f}, {0.5f, 0.4f});
    REQUIRE(get_subrect(rect, rel_subrect) == R({4.f, -3.f}, {5.f, -4.f}));
}

TEST_CASE("Rectangle: split (float)")
{
    using R = Xyz::RectangleF;
    const R rect({1.f, 2.f}, {6.f, 4.f});
    const auto cells = split(rect, 2, 3);
    REQUIRE(cells.size() == 6);
    // Row-major order: row 0 first, columns left to right.
    REQUIRE(cells[0] == R({1.f, 2.f}, {2.f, 2.f}));
    REQUIRE(cells[1] == R({3.f, 2.f}, {2.f, 2.f}));
    REQUIRE(cells[2] == R({5.f, 2.f}, {2.f, 2.f}));
    REQUIRE(cells[3] == R({1.f, 4.f}, {2.f, 2.f}));
    REQUIRE(cells[4] == R({3.f, 4.f}, {2.f, 2.f}));
    REQUIRE(cells[5] == R({5.f, 4.f}, {2.f, 2.f}));
}

TEST_CASE("Rectangle: split (int, even)")
{
    using R = Xyz::RectangleI;
    const auto cells = split(R({0, 0}, {10, 20}), 2, 2);
    REQUIRE(cells.size() == 4);
    REQUIRE(cells[0] == R({0, 0}, {5, 10}));
    REQUIRE(cells[1] == R({5, 0}, {5, 10}));
    REQUIRE(cells[2] == R({0, 10}, {5, 10}));
    REQUIRE(cells[3] == R({5, 10}, {5, 10}));
}

TEST_CASE("Rectangle: split (int, uneven) covers the whole rectangle")
{
    using R = Xyz::RectangleI;
    const R rect({1, 1}, {10, 7});
    const auto cells = split(rect, 2, 3);
    REQUIRE(cells.size() == 6);
    // Cells must tile the rectangle without gaps or overlaps.
    REQUIRE(cells[0] == R({1, 1}, {3, 3}));
    REQUIRE(cells[1] == R({4, 1}, {3, 3}));
    REQUIRE(cells[2] == R({7, 1}, {4, 3}));
    REQUIRE(cells[3] == R({1, 4}, {3, 4}));
    REQUIRE(cells[4] == R({4, 4}, {3, 4}));
    REQUIRE(cells[5] == R({7, 4}, {4, 4}));
    REQUIRE(get_max(cells.back()) == get_max(rect));
}

TEST_CASE("Rectangle: split with negative size")
{
    using R = Xyz::RectangleI;
    const auto cells = split(R({10, 10}, {-10, -4}), 2, 2);
    REQUIRE(cells.size() == 4);
    REQUIRE(cells[0] == R({10, 10}, {-5, -2}));
    REQUIRE(cells[1] == R({5, 10}, {-5, -2}));
    REQUIRE(cells[2] == R({10, 8}, {-5, -2}));
    REQUIRE(cells[3] == R({5, 8}, {-5, -2}));
}

TEST_CASE("Rectangle: split into single cell")
{
    using R = Xyz::RectangleI;
    const R rect({3, 4}, {5, 6});
    const auto cells = split(rect, 1, 1);
    REQUIRE(cells.size() == 1);
    REQUIRE(cells[0] == rect);
}

TEST_CASE("Rectangle: split with zero rows or columns")
{
    using R = Xyz::RectangleI;
    REQUIRE(split(R({0, 0}, {10, 10}), 0, 3).empty());
    REQUIRE(split(R({0, 0}, {10, 10}), 3, 0).empty());
}
