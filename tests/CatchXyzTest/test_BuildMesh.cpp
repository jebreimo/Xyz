//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-08-08.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Vector.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/MeshBuilder/MeshAttributeBuilder.hpp>

#include "Xyz/OrientedCuboid.hpp"
#include "Xyz/OrientedRectangle.hpp"
#include "Xyz/MeshBuilder/BuildMesh.hpp"

TEST_CASE("BuildMesh: make mesh from OrientedRectangle")
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

TEST_CASE("BuildMesh: make mesh from OrientedCuboid")
{
    std::vector<int32_t> indexes;
    std::vector<float> vertexes;
    std::vector<float> normals;
    std::vector<float> tangents;

    using Builder3F = Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>>;
    using Builder4F = Xyz::MeshAttributeBuilder<Xyz::Vector4F, std::vector<float>>;

    Xyz::MeshBuilder builder{
        .indexes = Xyz::MeshIndexBuilder<int32_t>(indexes),
        .coords = Builder3F(vertexes, 3),
        .normals = std::optional(Builder3F(normals, 3)),
        .tangents = std::optional(Builder4F(tangents, 4))
    };

    // Axis-aligned cuboid at the origin, 2 wide (x), 3 deep (y) and 4 tall (z).
    const Xyz::OrientedCuboid<float> cuboid{{{0, 0, 0}, {}}, {2, 3, 4}};
    Xyz::build_mesh(builder, cuboid);

    REQUIRE(vertexes.size() == 24 * 3);
    REQUIRE(normals.size() == 24 * 3);
    REQUIRE(tangents.size() == 24 * 4);
    REQUIRE(indexes.size() == 36);

    // The faces are added in the order left, front, right, back, top, bottom.
    constexpr Xyz::Vector3F expected_coords[24] = {
        {0, 3, 0}, {0, 0, 0}, {0, 0, 4}, {0, 3, 4}, // left (x = 0)
        {0, 0, 0}, {2, 0, 0}, {2, 0, 4}, {0, 0, 4}, // front (y = 0)
        {2, 0, 0}, {2, 3, 0}, {2, 3, 4}, {2, 0, 4}, // right (x = 2)
        {2, 3, 0}, {0, 3, 0}, {0, 3, 4}, {2, 3, 4}, // back (y = 3)
        {0, 0, 4}, {2, 0, 4}, {2, 3, 4}, {0, 3, 4}, // top (z = 4)
        {0, 3, 0}, {2, 3, 0}, {2, 0, 0}, {0, 0, 0}  // bottom (z = 0)
    };

    // The normals are unit vectors pointing out of the cuboid.
    constexpr Xyz::Vector3F expected_normals[6] = {
        {-1, 0, 0}, {0, -1, 0}, {1, 0, 0},
        {0, 1, 0}, {0, 0, 1}, {0, 0, -1}
    };

    // The tangents are unit vectors along the face's first edge, with the
    // handedness in the fourth component.
    constexpr Xyz::Vector4F expected_tangents[6] = {
        {0, -1, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1},
        {-1, 0, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}
    };

    for (size_t i = 0; i < 24; ++i)
    {
        CAPTURE(i);
        REQUIRE(builder.coords.get(i) == expected_coords[i]);
        REQUIRE(builder.normals->get(i) == expected_normals[i / 4]);
        REQUIRE(builder.tangents->get(i) == expected_tangents[i / 4]);
    }

    for (int32_t face = 0; face < 6; ++face)
    {
        CAPTURE(face);
        const auto b = face * 4;
        const auto i = face * 6;
        REQUIRE(indexes[i + 0] == b + 0);
        REQUIRE(indexes[i + 1] == b + 2);
        REQUIRE(indexes[i + 2] == b + 3);
        REQUIRE(indexes[i + 3] == b + 0);
        REQUIRE(indexes[i + 4] == b + 1);
        REQUIRE(indexes[i + 5] == b + 2);
    }

    // Every triangle must be wound so that its own normal agrees with the
    // face normal, i.e. counterclockwise as seen from outside the cuboid.
    for (size_t i = 0; i < indexes.size(); i += 3)
    {
        CAPTURE(i);
        const auto a = builder.coords.get(indexes[i]);
        const auto b = builder.coords.get(indexes[i + 1]);
        const auto c = builder.coords.get(indexes[i + 2]);
        REQUIRE(dot(cross(b - a, c - a), builder.normals->get(indexes[i])) > 0);
    }
}

TEST_CASE("BuildMesh: tangent handedness follows the texture rectangle")
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
