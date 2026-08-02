//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Mesh.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/MeshBuilder/MeshAttributeBuilder.hpp>

TEST_CASE("MeshAttributeBuilder: interleaved position and normal attributes")
{
    std::vector<float> buffer;

    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> pos_builder(buffer, 8);
    pos_builder.add(Xyz::Vector3F(1, 2, 3));
    pos_builder.add(Xyz::Vector3F(3, 4, 5));
    REQUIRE(buffer.size() == 16);
    REQUIRE(buffer[0] == 1);
    REQUIRE(buffer[1] == 2);
    REQUIRE(buffer[2] == 3);
    REQUIRE(buffer[8] == 3);
    REQUIRE(buffer[9] == 4);
    REQUIRE(buffer[10] == 5);

    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> nrm_builder(buffer, 8, 3);
    nrm_builder.add(Xyz::Vector3F(-1, 0, 1));
    nrm_builder.add(Xyz::Vector3F(0, 1, -1));
    REQUIRE(buffer.size() == 16);
    REQUIRE(buffer[3] == -1);
    REQUIRE(buffer[4] == 0);
    REQUIRE(buffer[5] == 1);
    REQUIRE(buffer[11] == 0);
    REQUIRE(buffer[12] == 1);
    REQUIRE(buffer[13] == -1);
}

TEST_CASE("MeshAttributeBuilder: get returns the value written by add")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8);
    builder.add(Xyz::Vector3F(1, 2, 3));
    builder.add(Xyz::Vector3F(4, 5, 6));

    REQUIRE(builder.get(0) == Xyz::Vector3F(1, 2, 3));
    REQUIRE(builder.get(1) == Xyz::Vector3F(4, 5, 6));
}

TEST_CASE("MeshAttributeBuilder: get respects the offset")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> pos(buffer, 8, 0);
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> nrm(buffer, 8, 3);
    pos.add(Xyz::Vector3F(1, 2, 3));
    nrm.add(Xyz::Vector3F(-1, 0, 1));

    REQUIRE(pos.get(0) == Xyz::Vector3F(1, 2, 3));
    REQUIRE(nrm.get(0) == Xyz::Vector3F(-1, 0, 1));
}

TEST_CASE("MeshAttributeBuilder: set overwrites an existing row")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8);
    builder.add(Xyz::Vector3F(1, 2, 3));
    builder.add(Xyz::Vector3F(4, 5, 6));

    builder.set(0, Xyz::Vector3F(7, 8, 9));

    REQUIRE(builder.get(0) == Xyz::Vector3F(7, 8, 9));
    REQUIRE(builder.get(1) == Xyz::Vector3F(4, 5, 6));
    REQUIRE(buffer.size() == 16);
}

TEST_CASE("MeshAttributeBuilder: add_n appends the same value n times")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8);
    builder.add(Xyz::Vector3F(1, 1, 1));
    builder.add_n(Xyz::Vector3F(2, 3, 4), 3);

    REQUIRE(buffer.size() == 4 * 8);
    REQUIRE(builder.get(0) == Xyz::Vector3F(1, 1, 1));
    REQUIRE(builder.get(1) == Xyz::Vector3F(2, 3, 4));
    REQUIRE(builder.get(2) == Xyz::Vector3F(2, 3, 4));
    REQUIRE(builder.get(3) == Xyz::Vector3F(2, 3, 4));
}

TEST_CASE("MeshAttributeBuilder: set_n writes a run of rows in place")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8);
    builder.add_n(Xyz::Vector3F(0, 0, 0), 4);

    builder.set_n(1, Xyz::Vector3F(5, 6, 7), 2);

    REQUIRE(builder.get(0) == Xyz::Vector3F(0, 0, 0));
    REQUIRE(builder.get(1) == Xyz::Vector3F(5, 6, 7));
    REQUIRE(builder.get(2) == Xyz::Vector3F(5, 6, 7));
    REQUIRE(builder.get(3) == Xyz::Vector3F(0, 0, 0));
}

TEST_CASE("MeshAttributeBuilder: initial row count and resize grows the buffer")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8, 0, 2);
    // Constructed with rows == 2 but an empty buffer; add() resizes from there.
    builder.add(Xyz::Vector3F(1, 2, 3));
    REQUIRE(buffer.size() == 3 * 8);
    REQUIRE(builder.get(2) == Xyz::Vector3F(1, 2, 3));
}

TEST_CASE("MeshAttributeBuilder: resize does not shrink the buffer")
{
    std::vector<float> buffer;
    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> builder(buffer, 8);
    builder.add_n(Xyz::Vector3F(1, 2, 3), 4);
    REQUIRE(buffer.size() == 4 * 8);

    builder.resize(2);
    REQUIRE(buffer.size() == 4 * 8);
}

TEST_CASE("MeshAttributeBuilder: constructor rejects a value too large for the stride")
{
    std::vector<float> buffer;
    // Vector3F needs 3 floats; a stride of 2 floats cannot hold it.
    REQUIRE_THROWS_AS(
        (Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>>(buffer, 2)),
        std::invalid_argument);
}

namespace
{
    struct Vec2f
    {
        float x, y;
    };

    bool operator==(const Vec2f& a, const Vec2f& b)
    {
        return a.x == b.x && a.y == b.y;
    }
}

TEST_CASE("MeshAttributeBuilder: works with a byte buffer and a custom value type")
{
    std::vector<std::byte> buffer;
    // Stride/offset are counted in buffer elements (bytes here).
    Xyz::MeshAttributeBuilder<Vec2f, std::vector<std::byte>> builder(
        buffer, sizeof(Vec2f) * 3, sizeof(Vec2f));
    builder.add(Vec2f{1.0f, 2.0f});
    builder.add(Vec2f{3.0f, 4.0f});

    REQUIRE(buffer.size() == 2 * sizeof(Vec2f) * 3);
    REQUIRE(builder.get(0) == Vec2f{1.0f, 2.0f});
    REQUIRE(builder.get(1) == Vec2f{3.0f, 4.0f});
}
