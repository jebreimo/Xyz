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

#include "Xyz/OrientedCuboid.hpp"
#include "Xyz/Mesh/CuboidMesh.hpp"

TEST_CASE("CuboidMesh: make mesh from OrientedCuboid")
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

    // The faces are added in the order left, front, right, back, bottom, top.
    constexpr Xyz::Vector3F expected_coords[24] = {
        {0, 0, 0}, {2, 0, 0}, {2, 0, 4}, {0, 0, 4}, // left (y = 0)
        {2, 0, 0}, {2, 3, 0}, {2, 3, 4}, {2, 0, 4}, // front (x = 2)
        {2, 3, 0}, {0, 3, 0}, {0, 3, 4}, {2, 3, 4}, // right (y = 3)
        {0, 3, 0}, {0, 0, 0}, {0, 0, 4}, {0, 3, 4}, // back (x = 0)
        {0, 3, 0}, {2, 3, 0}, {2, 0, 0}, {0, 0, 0}, // bottom (z = 0)
        {0, 0, 4}, {2, 0, 4}, {2, 3, 4}, {0, 3, 4}, // top (z = 4)
    };

    // The normals are unit vectors pointing out of the cuboid.
    constexpr Xyz::Vector3F expected_normals[6] = {
        {0, -1, 0}, {1, 0, 0}, {0, 1, 0},
        {-1, 0, 0}, {0, 0, -1}, {0, 0, 1}
    };

    // The tangents are unit vectors along the face's first edge, with the
    // handedness in the fourth component.
    constexpr Xyz::Vector4F expected_tangents[6] = {
        {1, 0, 0, 1}, {0, 1, 0, 1}, {-1, 0, 0, 1},
        {0, -1, 0, 1}, {1, 0, 0, 1}, {1, 0, 0, 1}
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

namespace
{
    struct CubeBuffers
    {
        std::vector<int32_t> indexes;
        std::vector<double> vertexes;
        std::vector<double> normals;
        std::vector<double> tangents;
        std::vector<double> tex_coords;

        Xyz::MeshBuilder<std::vector<double>, double, int32_t> make_builder()
        {
            using Builder2D = Xyz::MeshAttributeBuilder<Xyz::Vector2D, std::vector<double>>;
            using Builder3D = Xyz::MeshAttributeBuilder<Xyz::Vector3D, std::vector<double>>;
            using Builder4D = Xyz::MeshAttributeBuilder<Xyz::Vector4D, std::vector<double>>;
            return {
                .indexes = Xyz::MeshIndexBuilder<int32_t>(indexes),
                .coords = Builder3D(vertexes, 3),
                .normals = std::optional(Builder3D(normals, 3)),
                .tangents = std::optional(Builder4D(tangents, 4)),
                .tex_coords = std::optional(Builder2D(tex_coords, 2))
            };
        }
    };
}

TEST_CASE("CuboidMesh: build_cube geometry")
{
    constexpr double radius = 2.5;
    CubeBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_cube(builder, {.radius = radius});

    REQUIRE(buffers.vertexes.size() == 24 * 3);
    REQUIRE(buffers.normals.size() == 24 * 3);
    REQUIRE(buffers.tangents.size() == 24 * 4);
    REQUIRE(buffers.tex_coords.size() == 24 * 2);
    REQUIRE(buffers.indexes.size() == 36);

    // All vertexes lie on the circumscribed sphere.
    for (size_t i = 0; i < 24; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i)) == Xyz::Approx(radius));
    }

    // All edges have the same length.
    const auto edge_length = 2 * radius / std::sqrt(3.0);
    for (size_t face = 0; face < 6; ++face)
    {
        CAPTURE(face);
        for (size_t j = 0; j < 4; ++j)
        {
            CAPTURE(j);
            const auto a = builder.coords.get(face * 4 + j);
            const auto b = builder.coords.get(face * 4 + (j + 1) % 4);
            REQUIRE(Xyz::get_length(b - a) == Xyz::Approx(edge_length));
        }
    }

    // Without rotation, the faces are axis-aligned and come in the order
    // left, front, right, back, bottom, top.
    constexpr Xyz::Vector3D expected_normals[6] = {
        {0, -1, 0}, {1, 0, 0}, {0, 1, 0},
        {-1, 0, 0}, {0, 0, -1}, {0, 0, 1}
    };
    for (size_t i = 0; i < 24; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::are_equal(builder.normals->get(i), expected_normals[i / 4]));
    }
}

TEST_CASE("CuboidMesh: build_cube center and orientation")
{
    constexpr double radius = 2.0;
    const Xyz::Vector3D center(1, -2, 3);
    // Yawing by 45 degrees turns the front face's normal (the x-axis)
    // halfway towards the y-axis.
    const Xyz::OrientedRegularPolyhedron<double> shape{
        .center = center,
        .radius = radius,
        .orientation = {.yaw = Xyz::Constants<double>::PI / 4}
    };

    CubeBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_cube(builder, shape);

    for (size_t i = 0; i < 24; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i) - center) == Xyz::Approx(radius));
    }

    const auto s = std::sqrt(0.5);
    REQUIRE(Xyz::are_equal(builder.normals->get(4), Xyz::Vector3D(s, s, 0)));
    REQUIRE(Xyz::are_equal(builder.normals->get(20), Xyz::Vector3D(0, 0, 1)));

    // Normals point outwards from the center, and the triangles are wound
    // counterclockwise as seen from outside.
    for (size_t face = 0; face < 6; ++face)
    {
        CAPTURE(face);
        Xyz::Vector3D centroid;
        for (size_t j = 0; j < 4; ++j)
            centroid += builder.coords.get(face * 4 + j);
        centroid /= 4.0;
        REQUIRE(Xyz::are_equal(builder.normals->get(face * 4),
                               Xyz::normalize(centroid - center)));
    }

    for (size_t i = 0; i < buffers.indexes.size(); i += 3)
    {
        CAPTURE(i);
        const auto a = builder.coords.get(buffers.indexes[i]);
        const auto b = builder.coords.get(buffers.indexes[i + 1]);
        const auto c = builder.coords.get(buffers.indexes[i + 2]);
        REQUIRE(dot(cross(b - a, c - a), builder.normals->get(buffers.indexes[i])) > 0);
    }
}

TEST_CASE("CuboidMesh: build_cube texture rectangles and base_index")
{
    Xyz::Rectangle<double> tex_rects[2] = {
        {{0, 0}, {1, 1}},
        {{1, 0}, {1, 1}}
    };

    CubeBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_cube(builder, {}, std::span<Xyz::Rectangle<double>>(tex_rects), 100);

    REQUIRE(buffers.indexes.size() == 36);
    for (size_t i = 0; i < 36; ++i)
    {
        CAPTURE(i);
        REQUIRE(buffers.indexes[i] >= 100);
        REQUIRE(buffers.indexes[i] < 124);
    }

    // Fewer rectangles than faces: the rectangles are repeated in order.
    for (size_t face = 0; face < 6; ++face)
    {
        CAPTURE(face);
        const auto& rect = tex_rects[face % 2];
        REQUIRE(builder.tex_coords->get(face * 4) == rect.origin);
        REQUIRE(builder.tex_coords->get(face * 4 + 2) == rect.origin + rect.size);
    }
}

TEST_CASE("CuboidMesh: build_cube with float")
{
    std::vector<int32_t> indexes;
    std::vector<float> vertexes;

    using Builder3F = Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>>;
    Xyz::MeshBuilder builder{
        .indexes = Xyz::MeshIndexBuilder<int32_t>(indexes),
        .coords = Builder3F(vertexes, 3)
    };

    Xyz::build_cube(builder, {.radius = 1.0f});

    REQUIRE(vertexes.size() == 24 * 3);
    for (size_t i = 0; i < 24; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i)) == Xyz::Approx(1.0f));
    }
}
