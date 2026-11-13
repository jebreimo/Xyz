//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-13.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include "Xyz/Xyz.hpp"

using ValueType = std::variant<double, Xyz::Vector3D, Xyz::Vector4D, Xyz::Matrix3D, Xyz::Matrix4D,
                               std::string>;

using Variables = std::unordered_map<std::string, ValueType>;
