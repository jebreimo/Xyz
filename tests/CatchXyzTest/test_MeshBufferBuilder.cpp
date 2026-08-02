//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-07-29.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <Xyz/Mesh.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Xyz/MeshBufferBuilder.hpp>

TEST_CASE("MeshBufferBuilder: test 1")
{
    std::vector<float> buffer;

    Xyz::MeshAttributeBuilder<Xyz::Vector3F, std::vector<float>> pos_builder(buffer, 8);
    pos_builder.add(Xyz::Vector3F(1, 2, 3));
    REQUIRE(buffer.size() == 8);
    REQUIRE(buffer[0] == 1);
    REQUIRE(buffer[1] == 2);
    REQUIRE(buffer[2] == 3);
}
