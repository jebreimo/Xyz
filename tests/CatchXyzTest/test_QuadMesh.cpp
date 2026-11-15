//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-08.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Vector.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/Mesh/MeshAttributeBuilder.hpp>

#include "Xyz/OrientedRectangle.hpp"
#include "Xyz/Mesh/QuadMesh.hpp"

TEST_CASE("QuadMesh: make mesh from OrientedRectangle")
{
    std::vector<int32_t> indexes;
    std::vector<float> vertexes;
    std::vector<float> texture;

    using Builder3F = Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>>;
    using Builder2F = Xyz::MeshAttributeBuilder<Xyz::Vector2F, std::vector<float>>;

    Xyz::MeshBuilder builder{
        .indexes = Xyz::MeshIndexBuilder<int32_t>(indexes),
        .coords = Builder3F(vertexes, 3),
        .tex_coords = std::optional(Builder2F(texture, 2))
    };

    Xyz::OrientedRectangle3F rect({{1, 2, 3}, {0, 0, 0}}, {10, 20});
    Xyz::build_mesh(builder, rect, {{0, 0}, {1, 1}});

    REQUIRE(indexes.size() == 6);
    REQUIRE(vertexes.size() == 4 * 3);
    REQUIRE(vertexes[0] == 1);
    REQUIRE(vertexes[1] == 2);
    REQUIRE(vertexes[2] == 3);
    REQUIRE(vertexes[3] == 11);
    REQUIRE(vertexes[4] == 2);
    REQUIRE(vertexes[5] == 3);
    REQUIRE(vertexes[6] == 11);
    REQUIRE(vertexes[7] == 22);
    REQUIRE(vertexes[8] == 3);
    REQUIRE(vertexes[9] == 1);
    REQUIRE(vertexes[10] == 22);
    REQUIRE(vertexes[11] == 3);

    REQUIRE(texture.size() == 4 * 2);
    REQUIRE(texture[0] == 0);
    REQUIRE(texture[1] == 0);
    REQUIRE(texture[2] == 1);
    REQUIRE(texture[3] == 0);
    REQUIRE(texture[4] == 1);
    REQUIRE(texture[5] == 1);
    REQUIRE(texture[6] == 0);
    REQUIRE(texture[7] == 1);
}

TEST_CASE("QuadMesh: tangent handedness follows the texture rectangle")
{
    std::vector<int32_t> indexes;
    std::vector<float> vertexes;
    std::vector<float> tangents;

    using Builder3F = Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>>;
    using Builder4F = Xyz::MeshAttributeBuilder<Xyz::Vector4F, std::vector<float>>;

    Xyz::MeshBuilder builder{
        .indexes = Xyz::MeshIndexBuilder<int32_t>(indexes),
        .coords = Builder3F(vertexes, 3),
        .tangents = std::optional(Builder4F(tangents, 4))
    };

    // A rectangle in the xy-plane, so the edges are {10, 0, 0} and {0, 20, 0}
    // and the normal is {0, 0, 1}.
    const Xyz::OrientedRectangle3F rect({{0, 0, 0}, {}}, {10, 20});

    Xyz::Rectangle<float> tex_rect;
    Xyz::Vector4F expected;

    SECTION("unmirrored")
    {
        tex_rect = {{0, 0}, {1, 1}};
        expected = {1, 0, 0, 1};
    }
    SECTION("mirrored u")
    {
        tex_rect = {{1, 0}, {-1, 1}};
        expected = {-1, 0, 0, -1};
    }
    SECTION("mirrored v")
    {
        tex_rect = {{0, 1}, {1, -1}};
        expected = {1, 0, 0, -1};
    }
    SECTION("mirrored u and v")
    {
        tex_rect = {{1, 1}, {-1, -1}};
        expected = {-1, 0, 0, 1};
    }

    Xyz::build_mesh(builder, rect, tex_rect);

    REQUIRE(tangents.size() == 4 * 4);
    for (size_t i = 0; i < 4; ++i)
    {
        CAPTURE(i);
        REQUIRE(builder.tangents->get(i) == expected);
    }
}
