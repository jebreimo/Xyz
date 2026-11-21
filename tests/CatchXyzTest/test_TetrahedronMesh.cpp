//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-16.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Vector.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/Approx.hpp>
#include <Xyz/Mesh/MeshAttributeBuilder.hpp>

#include "Xyz/Mesh/TetrahedronMesh.hpp"

namespace
{
    using Builder2D = Xyz::MeshAttributeBuilder<Xyz::Vector2D, std::vector<double>>;
    using Builder3D = Xyz::MeshAttributeBuilder<Xyz::Vector3D, std::vector<double>>;
    using Builder4D = Xyz::MeshAttributeBuilder<Xyz::Vector4D, std::vector<double>>;

    struct TetrahedronBuffers
    {
        std::vector<int32_t> indexes;
        std::vector<double> vertexes;
        std::vector<double> normals;
        std::vector<double> tangents;
        std::vector<double> tex_coords;

        Xyz::MeshBuilder<std::vector<double>, double, int32_t> make_builder()
        {
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

TEST_CASE("TetrahedronMesh: buffer sizes")
{
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder);

    REQUIRE(buffers.vertexes.size() == 12 * 3);
    REQUIRE(buffers.normals.size() == 12 * 3);
    REQUIRE(buffers.tangents.size() == 12 * 4);
    REQUIRE(buffers.tex_coords.size() == 12 * 2);
    REQUIRE(buffers.indexes.size() == 12);
}

TEST_CASE("TetrahedronMesh: geometry")
{
    constexpr double radius = 2.5;
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder, {.radius = radius});

    // All vertexes lie on the circumscribed sphere.
    for (size_t i = 0; i < 12; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i)) == Xyz::Approx(radius));
    }

    // The tetrahedron is regular: all edges have the same length.
    const auto edge_length = radius * std::sqrt(8.0 / 3.0);
    for (size_t face = 0; face < 4; ++face)
    {
        CAPTURE(face);
        for (size_t j = 0; j < 3; ++j)
        {
            CAPTURE(j);
            const auto a = builder.coords.get(face * 3 + j);
            const auto b = builder.coords.get(face * 3 + (j + 1) % 3);
            REQUIRE(Xyz::get_length(b - a) == Xyz::Approx(edge_length));
        }
    }

    // The top vertex is on the z-axis.
    REQUIRE(Xyz::are_equal(builder.coords.get(5), Xyz::Vector3D(0, 0, radius)));
}

TEST_CASE("TetrahedronMesh: center and orientation")
{
    constexpr double radius = 2.0;
    const Xyz::Vector3D center(1, -2, 3);
    // Pitching by 90 degrees turns the z-axis into the x-axis.
    const Xyz::OrientedRegularPolyhedron<double> polyhedron{
        .center = center,
        .radius = radius,
        .orientation = {.pitch = Xyz::Constants<double>::PI / 2}
    };

    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder, polyhedron);

    for (size_t i = 0; i < 12; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i) - center) == Xyz::Approx(radius));
    }

    // The top vertex has been rotated onto the x-axis through the center.
    REQUIRE(Xyz::are_equal(builder.coords.get(5), center + Xyz::Vector3D(radius, 0, 0)));

    // Normals still point outwards from the center.
    for (size_t face = 0; face < 4; ++face)
    {
        CAPTURE(face);
        const auto a = builder.coords.get(face * 3);
        const auto b = builder.coords.get(face * 3 + 1);
        const auto c = builder.coords.get(face * 3 + 2);
        const auto centroid = (a + b + c) / 3.0;
        REQUIRE(Xyz::are_equal(builder.normals->get(face * 3),
                               Xyz::normalize(centroid - center)));
    }
}

TEST_CASE("TetrahedronMesh: normals point outwards")
{
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder);

    for (size_t face = 0; face < 4; ++face)
    {
        CAPTURE(face);
        const auto a = builder.coords.get(face * 3);
        const auto b = builder.coords.get(face * 3 + 1);
        const auto c = builder.coords.get(face * 3 + 2);
        const auto centroid = (a + b + c) / 3.0;
        for (size_t j = 0; j < 3; ++j)
        {
            CAPTURE(j);
            const auto n = builder.normals->get(face * 3 + j);
            REQUIRE(Xyz::get_length(n) == Xyz::Approx(1.0));
            // The tetrahedron is centered at the origin, so an outward
            // normal points in the same direction as the face's centroid.
            REQUIRE(Xyz::are_equal(n, Xyz::normalize(centroid)));
        }
    }
}

TEST_CASE("TetrahedronMesh: triangles are wound counterclockwise")
{
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder);

    // Each face gets its own three vertexes.
    for (int32_t i = 0; i < 12; ++i)
        REQUIRE(buffers.indexes[i] == i);

    // Every triangle must be wound so that its own normal agrees with the
    // face normal, i.e. counterclockwise as seen from outside.
    for (size_t i = 0; i < buffers.indexes.size(); i += 3)
    {
        CAPTURE(i);
        const auto a = builder.coords.get(buffers.indexes[i]);
        const auto b = builder.coords.get(buffers.indexes[i + 1]);
        const auto c = builder.coords.get(buffers.indexes[i + 2]);
        REQUIRE(dot(cross(b - a, c - a), builder.normals->get(buffers.indexes[i])) > 0);
    }
}

TEST_CASE("TetrahedronMesh: base_index offsets indexes")
{
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder, {}, {}, 100);

    REQUIRE(buffers.indexes.size() == 12);
    for (int32_t i = 0; i < 12; ++i)
        REQUIRE(buffers.indexes[i] == 100 + i);
}

TEST_CASE("TetrahedronMesh: texture coordinates")
{
    using Tri2 = Xyz::Triangle<double, 2>;
    Tri2 tex_maps[2] = {
        Tri2({0, 0}, {1, 0}, {0, 1}),
        Tri2({0.5, 0.5}, {1, 0.5}, {0.5, 1})
    };

    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder, {}, std::span<Tri2>(tex_maps));

    REQUIRE(buffers.tex_coords.size() == 12 * 2);

    // Fewer maps than faces: the maps are repeated in order.
    for (size_t face = 0; face < 4; ++face)
    {
        CAPTURE(face);
        const auto& expected = tex_maps[face % 2];
        for (unsigned j = 0; j < 3; ++j)
        {
            CAPTURE(j);
            REQUIRE(builder.tex_coords->get(face * 3 + j) == expected[j]);
        }
    }
}

TEST_CASE("TetrahedronMesh: without texture maps")
{
    TetrahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_tetrahedron(builder);

    for (size_t i = 0; i < 12; ++i)
    {
        CAPTURE(i);
        REQUIRE(builder.tex_coords->get(i) == Xyz::Vector2D());
    }
}
