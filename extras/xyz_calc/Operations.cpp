//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-02-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Operations.hpp"
#include <vector>

bool addition(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for addition.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<double>(a) + std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) &&
        std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) + std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) &&
        std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) + std::get<Xyz::Vector4D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix3D>(a) &&
        std::holds_alternative<Xyz::Matrix3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix3D>(a) + std::get<Xyz::Matrix3D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(a) &&
        std::holds_alternative<Xyz::Matrix4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix4D>(a) + std::get<Xyz::Matrix4D>(b));
    }
    else
    {
        XYZ_THROW("Incompatible types for addition.");
    }
    return true;
}

bool subtraction(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for subtraction.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<double>(a) - std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) &&
        std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) - std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) &&
        std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) - std::get<Xyz::Vector4D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix3D>(a) &&
        std::holds_alternative<Xyz::Matrix3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix3D>(a) - std::get<Xyz::Matrix3D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(a) &&
        std::holds_alternative<Xyz::Matrix4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix4D>(a) - std::get<Xyz::Matrix4D>(b));
    }
    else
    {
        XYZ_THROW("Incompatible types for subtraction.");
    }
    return true;
}

bool multiplication(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for multiplication.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<double>(a) * std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) * std::get<double>(b));
    }
    else if (std::holds_alternative<double>(a) && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<double>(a) * std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) * std::get<double>(b));
    }
    else if (std::holds_alternative<double>(a) && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<double>(a) * std::get<Xyz::Vector4D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) * std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) * std::get<Xyz::Vector4D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) * std::get<Xyz::Vector4D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix3D>(a) && std::holds_alternative<Xyz::Matrix3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix3D>(a) * std::get<Xyz::Matrix3D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(a) && std::holds_alternative<Xyz::Matrix4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix4D>(a) * std::get<Xyz::Matrix4D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix3D>(a) && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix3D>(a) * std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(a) && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix4D>(a) * std::get<Xyz::Vector4D>(b));
    }
    else
    {
        XYZ_THROW("Incompatible types for multiplication.");
    }
    return true;
}

bool inner_product(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for inner product.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<Xyz::Vector3D>(a)
        && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(Xyz::dot(std::get<Xyz::Vector3D>(a),
                                    std::get<Xyz::Vector3D>(b)));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a)
        && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(Xyz::dot(std::get<Xyz::Vector4D>(a),
                                    std::get<Xyz::Vector4D>(b)));
    }
    else
    {
        XYZ_THROW("Incompatible types for inner product.");
    }
    return true;
}

namespace
{
    std::vector<double> get_doubles(std::vector<ValueType>& stack, size_t n)
    {
        if (stack.size() < n)
        {
            XYZ_THROW("Not enough values on stack. Stack size: "
                + std::to_string(stack.size())
                + ", required: " + std::to_string(n) + ".");
        }

        std::vector<double> values;
        values.reserve(n);
        for (size_t i = stack.size() - n; i < stack.size(); i++)
        {
            if (!std::holds_alternative<double>(stack[i]))
            {
                XYZ_THROW("Expected a double on the stack at index "
                    + std::to_string(stack.size() - i - 1) + ".");
            }
            values.push_back(std::get<double>(stack[i]));
        }
        stack.resize(stack.size() - n);
        return values;
    }
}

bool vec3(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 3);
    stack.emplace_back(Xyz::Vector3D(values[0], values[1], values[2]));
    return true;
}

bool vec4(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 4);
    stack.emplace_back(Xyz::Vector4D(values[0], values[1], values[2], values[3]));
    return true;
}

bool identity3(std::vector<ValueType>& stack)
{
    stack.emplace_back(Xyz::Matrix3D::identity());
    return true;
}

bool identity4(std::vector<ValueType>& stack)
{
    stack.emplace_back(Xyz::Matrix4D::identity());
    return true;
}

bool rotate2(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 1);
    stack.emplace_back(Xyz::affine::rotate2(values[0]));
    return true;
}

bool rotate_x(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 1);
    stack.emplace_back(Xyz::affine::rotate_x(values[0]));
    return true;
}

bool rotate_y(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 1);
    stack.emplace_back(Xyz::affine::rotate_y(values[0]));
    return true;
}

bool rotate_z(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 1);
    stack.emplace_back(Xyz::affine::rotate_z(values[0]));
    return true;
}

bool translate2(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 2);
    stack.emplace_back(Xyz::affine::translate2(
        Xyz::Vector2D(values[0], values[1])));
    return true;
}

bool translate3(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 3);
    stack.emplace_back(Xyz::affine::translate3(
        Xyz::Vector3D(values[0], values[1], values[2])));
    return true;
}

bool scale2(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 2);
    stack.emplace_back(Xyz::affine::scale2(
        Xyz::Vector2D(values[0], values[1])));
    return true;
}

bool scale3(std::vector<ValueType>& stack)
{
    const auto values = get_doubles(stack, 3);
    stack.emplace_back(Xyz::affine::scale3(
        Xyz::Vector3D(values[0], values[1], values[2])));
    return true;
}
bool transpose(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for transpose.");

    auto value = stack.back();
    stack.pop_back();

    if (std::holds_alternative<Xyz::Matrix3D>(value))
    {
        stack.emplace_back(transpose(std::get<Xyz::Matrix3D>(value)));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(value))
    {
        stack.emplace_back(transpose(std::get<Xyz::Matrix4D>(value)));
    }
    else
    {
        XYZ_THROW("Incompatible type for transpose.");
    }
    return true;
}

bool invert(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for invert.");

    auto value = stack.back();
    stack.pop_back();

    if (std::holds_alternative<Xyz::Matrix3D>(value))
    {
        stack.emplace_back(invert(std::get<Xyz::Matrix3D>(value)));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(value))
    {
        stack.emplace_back(invert(std::get<Xyz::Matrix4D>(value)));
    }
    else
    {
        XYZ_THROW("Incompatible type for invert.");
    }
    return true;
}

bool duplicate(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for duplicate.");

    stack.push_back(stack.back());
    return true;
}

bool print(std::ostream& stream, const ValueType& value)
{
    std::visit([&stream](const auto& v)
    {
        stream << v;
    }, value);
    return true;
}
