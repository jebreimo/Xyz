//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-10-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <catch2/catch_test_macros.hpp>

#include "Xyz/Mesh/MeshUtilities.hpp"

TEST_CASE("MeshUtilities: triangles_to_line_indexes with no triangles")
{
    std::vector<uint32_t> indexes;
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines.empty());
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes with single triangle")
{
    std::vector<uint32_t> indexes = {0, 1, 2};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes removes shared edges")
{
    // A quad made of two triangles sharing the diagonal 0-2.
    std::vector<uint32_t> indexes = {0, 1, 2, 0, 2, 3};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 0, 3, 1, 2, 2, 3});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes treats opposite winding as same edge")
{
    std::vector<uint32_t> indexes = {0, 1, 2, 2, 1, 0};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes on tetrahedron")
{
    std::vector<uint32_t> indexes = {0, 1, 2,
                                     0, 3, 1,
                                     1, 3, 2,
                                     2, 3, 0};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes ignores incomplete trailing triangle")
{
    std::vector<uint32_t> indexes = {0, 1, 2, 3, 4};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes with large uint32 indexes")
{
    std::vector<uint32_t> indexes = {0xFFFFFFFF, 0, 0x80000000};
    auto lines = Xyz::triangles_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 0x80000000,
                                           0, 0xFFFFFFFF,
                                           0x80000000, 0xFFFFFFFF});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes with uint16 indexes")
{
    std::vector<uint16_t> indexes = {2, 1, 0, 3, 2, 0};
    auto lines = Xyz::triangles_to_line_indexes<uint16_t>(indexes);
    REQUIRE(lines == std::vector<uint16_t>{0, 1, 0, 2, 0, 3, 1, 2, 2, 3});
}

TEST_CASE("MeshUtilities: triangles_to_line_indexes with int32 indexes")
{
    std::vector<int32_t> indexes = {4, 5, 6};
    auto lines = Xyz::triangles_to_line_indexes<int32_t>(indexes);
    REQUIRE(lines == std::vector<int32_t>{4, 5, 4, 6, 5, 6});
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes with fewer than three indexes")
{
    std::vector<uint32_t> indexes;
    REQUIRE(Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes).empty());
    indexes = {0};
    REQUIRE(Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes).empty());
    indexes = {0, 1};
    REQUIRE(Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes).empty());
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes with single triangle")
{
    std::vector<uint32_t> indexes = {0, 1, 2};
    auto lines = Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2});
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes with several triangles")
{
    // Every triangle shares the first index; the spokes are only added once.
    std::vector<uint32_t> indexes = {0, 1, 2, 3, 4};
    auto lines = Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 0, 3, 0, 4,
                                           1, 2, 2, 3, 3, 4});
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes removes duplicate edge in closed fan")
{
    // The last index repeats the second one, closing the fan.
    std::vector<uint32_t> indexes = {0, 1, 2, 3, 4, 1};
    auto lines = Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 0, 3, 0, 4,
                                           1, 2, 1, 4, 2, 3, 3, 4});
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes orders each edge and sorts edges")
{
    std::vector<uint32_t> indexes = {5, 3, 9, 1};
    auto lines = Xyz::triangle_fan_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{1, 5, 1, 9, 3, 5, 3, 9, 5, 9});
}

TEST_CASE("MeshUtilities: triangle_fan_to_line_indexes with uint16 indexes")
{
    std::vector<uint16_t> indexes = {0, 1, 2, 3};
    auto lines = Xyz::triangle_fan_to_line_indexes<uint16_t>(indexes);
    REQUIRE(lines == std::vector<uint16_t>{0, 1, 0, 2, 0, 3, 1, 2, 2, 3});
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes with fewer than three indexes")
{
    std::vector<uint32_t> indexes;
    REQUIRE(Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes).empty());
    indexes = {0};
    REQUIRE(Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes).empty());
    indexes = {0, 1};
    REQUIRE(Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes).empty());
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes with single triangle")
{
    std::vector<uint32_t> indexes = {0, 1, 2};
    auto lines = Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2});
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes with several triangles")
{
    // Each new index is connected to the two preceding ones.
    std::vector<uint32_t> indexes = {0, 1, 2, 3, 4};
    auto lines = Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 1, 2, 1, 3,
                                           2, 3, 2, 4, 3, 4});
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes removes duplicate edges in closed strip")
{
    // The last two indexes repeat the first two, closing the strip.
    std::vector<uint32_t> indexes = {0, 1, 2, 3, 0, 1};
    auto lines = Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{0, 1, 0, 2, 0, 3, 1, 2, 1, 3, 2, 3});
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes orders each edge and sorts edges")
{
    std::vector<uint32_t> indexes = {9, 4, 7, 2};
    auto lines = Xyz::triangle_strip_to_line_indexes<uint32_t>(indexes);
    REQUIRE(lines == std::vector<uint32_t>{2, 4, 2, 7, 4, 7, 4, 9, 7, 9});
}

TEST_CASE("MeshUtilities: triangle_strip_to_line_indexes with uint16 indexes")
{
    std::vector<uint16_t> indexes = {0, 1, 2, 3};
    auto lines = Xyz::triangle_strip_to_line_indexes<uint16_t>(indexes);
    REQUIRE(lines == std::vector<uint16_t>{0, 1, 0, 2, 1, 2, 1, 3, 2, 3});
}
