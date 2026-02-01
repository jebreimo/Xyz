//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-02-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Xyz/Xyz.hpp"

using ValueType = std::variant<double, Xyz::Vector3D, Xyz::Vector4D, Xyz::Matrix3D, Xyz::Matrix4D>;

bool addition(std::vector<ValueType>& stack);

bool subtraction(std::vector<ValueType>& stack);

bool multiplication(std::vector<ValueType>& stack);

bool inner_product(std::vector<ValueType>& stack);

bool vec2(std::vector<ValueType>& stack);

bool vec3(std::vector<ValueType>& stack);

bool vec4(std::vector<ValueType>& stack);

bool identity3(std::vector<ValueType>& stack);

bool identity4(std::vector<ValueType>& stack);

bool rotate2(std::vector<ValueType>& stack);

bool rotate_x(std::vector<ValueType>& stack);

bool rotate_y(std::vector<ValueType>& stack);

bool rotate_z(std::vector<ValueType>& stack);

bool translate2(std::vector< ValueType>& stack);

bool translate3(std::vector< ValueType>& stack);

bool scale2(std::vector<ValueType>& stack);

bool scale3(std::vector<ValueType>& stack);

bool transpose(std::vector<ValueType>& stack);

bool invert(std::vector<ValueType>& stack);

bool duplicate(std::vector<ValueType>& stack);

bool print(std::ostream& stream, const ValueType& value);
