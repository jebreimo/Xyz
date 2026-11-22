//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-21.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Vector.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/Approx.hpp>
#include <Xyz/Mesh/MeshAttributeBuilder.hpp>

#include "Xyz/Mesh/DodecahedronMesh.hpp"

namespace
{
    using Builder2D = Xyz::MeshAttributeBuilder<Xyz::Vector2D, std::vector<double>>;
    using Builder3D = Xyz::MeshAttributeBuilder<Xyz::Vector3D, std::vector<double>>;
    using Builder4D = Xyz::MeshAttributeBuilder<Xyz::Vector4D, std::vector<double>>;

    struct DodecahedronBuffers
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

TEST_CASE("DodecahedronMesh: buffer sizes")
{
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder);

    REQUIRE(buffers.vertexes.size() == 60 * 3);
    REQUIRE(buffers.normals.size() == 60 * 3);
    REQUIRE(buffers.tangents.size() == 60 * 4);
    REQUIRE(buffers.tex_coords.size() == 60 * 2);
    REQUIRE(buffers.indexes.size() == 12 * 9);
}

TEST_CASE("DodecahedronMesh: geometry")
{
    constexpr double radius = 2.5;
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder, {.radius = radius});

    // All vertexes lie on the circumscribed sphere.
    for (size_t i = 0; i < 60; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i)) == Xyz::Approx(radius));
    }

    // The dodecahedron is regular: all edges have the same length, and
    // every face is planar.
    const auto edge_length = radius * (std::sqrt(5.0) - 1) / std::sqrt(3.0);
    for (size_t face = 0; face < 12; ++face)
    {
        CAPTURE(face);
        const auto n = builder.normals->get(face * 5);
        const auto d = dot(n, builder.coords.get(face * 5));
        for (size_t j = 0; j < 5; ++j)
        {
            CAPTURE(j);
            const auto a = builder.coords.get(face * 5 + j);
            const auto b = builder.coords.get(face * 5 + (j + 1) % 5);
            REQUIRE(Xyz::get_length(b - a) == Xyz::Approx(edge_length));
            REQUIRE(dot(n, a) == Xyz::Approx(d));
        }
    }

    // The first face is the top face and the last face is the bottom face.
    REQUIRE(Xyz::are_equal(builder.normals->get(0), Xyz::Vector3D(0, 0, 1)));
    REQUIRE(Xyz::are_equal(builder.normals->get(55), Xyz::Vector3D(0, 0, -1)));
}

TEST_CASE("DodecahedronMesh: center and orientation")
{
    constexpr double radius = 2.0;
    const Xyz::Vector3D center(1, -2, 3);
    // Pitching by 90 degrees turns the z-axis into the x-axis.
    const Xyz::OrientedRegularPolyhedron<double> polyhedron{
        .center = center,
        .radius = radius,
        .orientation = {.pitch = Xyz::Constants<double>::PI / 2}
    };

    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder, polyhedron);

    for (size_t i = 0; i < 60; ++i)
    {
        CAPTURE(i);
        REQUIRE(Xyz::get_length(builder.coords.get(i) - center) == Xyz::Approx(radius));
    }

    // The top face now faces along the x-axis.
    REQUIRE(Xyz::are_equal(builder.normals->get(0), Xyz::Vector3D(1, 0, 0)));

    // Normals still point outwards from the center.
    for (size_t face = 0; face < 12; ++face)
    {
        CAPTURE(face);
        Xyz::Vector3D centroid;
        for (size_t j = 0; j < 5; ++j)
            centroid += builder.coords.get(face * 5 + j);
        centroid /= 5.0;
        REQUIRE(Xyz::are_equal(builder.normals->get(face * 5),
                               Xyz::normalize(centroid - center)));
    }
}

TEST_CASE("DodecahedronMesh: normals point outwards")
{
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder);

    for (size_t face = 0; face < 12; ++face)
    {
        CAPTURE(face);
        Xyz::Vector3D centroid;
        for (size_t j = 0; j < 5; ++j)
            centroid += builder.coords.get(face * 5 + j);
        centroid /= 5.0;
        for (size_t j = 0; j < 5; ++j)
        {
            CAPTURE(j);
            const auto n = builder.normals->get(face * 5 + j);
            REQUIRE(Xyz::get_length(n) == Xyz::Approx(1.0));
            // The dodecahedron is centered at the origin, so an outward
            // normal points in the same direction as the face's centroid.
            REQUIRE(Xyz::are_equal(n, Xyz::normalize(centroid)));
        }
    }
}

TEST_CASE("DodecahedronMesh: triangles are wound counterclockwise")
{
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder);

    // Each face gets its own five vertexes, split into a fan of three
    // triangles.
    constexpr int32_t fan[] = {0, 1, 2, 0, 2, 3, 0, 3, 4};
    for (int32_t face = 0; face < 12; ++face)
    {
        CAPTURE(face);
        for (int32_t j = 0; j < 9; ++j)
            REQUIRE(buffers.indexes[face * 9 + j] == face * 5 + fan[j]);
    }

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

TEST_CASE("DodecahedronMesh: base_index offsets indexes")
{
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder, {}, {}, 100);

    DodecahedronBuffers reference_buffers;
    auto reference_builder = reference_buffers.make_builder();
    Xyz::build_dodecahedron(reference_builder);

    REQUIRE(buffers.indexes.size() == reference_buffers.indexes.size());
    for (size_t i = 0; i < buffers.indexes.size(); ++i)
        REQUIRE(buffers.indexes[i] == 100 + reference_buffers.indexes[i]);
}

TEST_CASE("DodecahedronMesh: texture coordinates")
{
    using Pent2 = Xyz::Pentagon<double, 2>;
    Pent2 tex_maps[5] = {
        Pent2({0.5, 1}, {0, 0.6}, {0.2, 0}, {0.8, 0}, {1, 0.6}),
        Pent2({1.5, 1}, {1, 0.6}, {1.2, 0}, {1.8, 0}, {2, 0.6}),
        Pent2({2.5, 1}, {2, 0.6}, {2.2, 0}, {2.8, 0}, {3, 0.6}),
        Pent2({3.5, 1}, {3, 0.6}, {3.2, 0}, {3.8, 0}, {4, 0.6}),
        Pent2({4.5, 1}, {4, 0.6}, {4.2, 0}, {4.8, 0}, {5, 0.6})
    };

    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder, {}, std::span<Pent2>(tex_maps));

    REQUIRE(buffers.tex_coords.size() == 60 * 2);

    // Fewer maps than faces: the maps are repeated in order.
    for (size_t face = 0; face < 12; ++face)
    {
        CAPTURE(face);
        const auto& expected = tex_maps[face % 5];
        for (unsigned j = 0; j < 5; ++j)
        {
            CAPTURE(j);
            REQUIRE(builder.tex_coords->get(face * 5 + j) == expected[j]);
        }
    }
}

TEST_CASE("DodecahedronMesh: without texture maps")
{
    DodecahedronBuffers buffers;
    auto builder = buffers.make_builder();
    Xyz::build_dodecahedron(builder);

    for (size_t i = 0; i < 60; ++i)
    {
        CAPTURE(i);
        REQUIRE(builder.tex_coords->get(i) == Xyz::Vector2D());
    }
}
