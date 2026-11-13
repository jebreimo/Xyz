//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-02-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "TypeDefs.hpp"

bool addition(std::vector<ValueType>& stack);

bool subtraction(std::vector<ValueType>& stack);

bool multiplication(std::vector<ValueType>& stack);

bool division(std::vector<ValueType>& stack);

bool negate(std::vector<ValueType>& stack);

bool square_root(std::vector<ValueType>& stack);

bool power(std::vector<ValueType>& stack);

bool sine(std::vector<ValueType>& stack);

bool cosine(std::vector<ValueType>& stack);

bool tangent(std::vector<ValueType>& stack);

bool arcsine(std::vector<ValueType>& stack);

bool arccosine(std::vector<ValueType>& stack);

bool arctangent(std::vector<ValueType>& stack);

bool arctangent2(std::vector<ValueType>& stack);

bool logarithm(std::vector<ValueType>& stack);

bool inner_product(std::vector<ValueType>& stack);

bool cross_product(std::vector<ValueType>& stack);

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

bool normalize(std::vector<ValueType>& stack);

bool duplicate(std::vector<ValueType>& stack);

bool swap(std::vector<ValueType>& stack);

bool rotate(std::vector<ValueType>& stack);

bool pop(std::vector<ValueType>& stack);

bool clear(std::vector<ValueType>& stack);

bool print_stack(std::vector<ValueType>& stack);

bool matrix3(std::vector<ValueType>& stack);

bool print(std::ostream& stream, const ValueType& value);
