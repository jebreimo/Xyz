//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-13.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Operations.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace
{
    double clean(double value)
    {
        if (value == -0.0)
            value = 0.0;

        const auto whole = std::round(value);
        if (std::fabs(value - whole) < 1e-10)
            return whole;
        return value;
    }

    template<typename T, unsigned N>
    Xyz::Vector<T, N> clean(const Xyz::Vector<T, N>& vector)
    {
        Xyz::Vector<T, N> result;
        for (unsigned i = 0; i < N; ++i)
        {
            result.values[i] = clean(vector.values[i]);
        }
        return result;
    }

    template<typename T, unsigned M, unsigned N>
    Xyz::Matrix<T, M, N> clean(const Xyz::Matrix<T, M, N>& matrix)
    {
        Xyz::Matrix<T, M, N> result;
        for (unsigned i = 0; i < M * N; ++i)
        {
            result.values[i] = clean(matrix.values[i]);
        }
        return result;
    }

    ValueType clean(ValueType value)
    {
        return std::visit([](auto& v) -> ValueType
        {
            return clean(v);
        }, value);
    }

    template <typename ValueT1, typename ValueT2, typename ResultT>
    void print_op(const ValueT1& a, const ValueT2& b,
                  const ResultT& result, const std::string& op_name)
    {
        std::cout << clean(a) << " " << op_name << " " << clean(b) << " = " << clean(result) << std::endl;
    }

    template <typename ValueT1, typename ResultT>
    void print_op(const ValueT1& a, const ResultT& result,
                  const std::string& op_name)
    {
        std::cout << op_name << " " << clean(a) << " = " << clean(result) << std::endl;
    }

    bool print(const ValueType& value)
    {
        std::visit([](const auto& v)
        {
            std::cout << clean(v) << std::endl;
        }, value);
        return true;
    }
}

void UnaryOperation::operator()(std::vector<ValueType>& stack) const
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for " + name + ".");

    const auto a = stack.back();
    stack.pop_back();

    if (!execute(a, stack))
        XYZ_THROW("Incompatible types for " + name + ".");
}

template <typename T, typename Op>
bool UnaryOperation::unary_op(const ValueType& a,
                              const Op& op,
                              std::vector<ValueType>& stack) const
{
    if (std::holds_alternative<T>(a))
    {
        const auto& lhs = std::get<T>(a);
        auto result = op(lhs);
        print_op(lhs, result, name);
        stack.emplace_back(result);
        return true;
    }
    return false;
}

void BinaryOperation::operator()(std::vector<ValueType>& stack) const
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for " + name + ".");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    if (!execute(a, b, stack))
        XYZ_THROW("Incompatible types for " + name + ".");
}

template <typename T, typename U, typename Op>
bool BinaryOperation::binary_op(const ValueType& a,
                                const ValueType& b,
                                const Op& op,
                                std::vector<ValueType>& stack) const
{
    if (std::holds_alternative<T>(a) && std::holds_alternative<U>(b))
    {
        const auto& lhs = std::get<T>(a);
        const auto& rhs = std::get<U>(b);
        auto result = op(lhs, rhs);
        print_op(lhs, rhs, result, name);
        stack.emplace_back(result);
        return true;
    }
    return false;
}

bool Addition::execute(const ValueType& a,
                       const ValueType& b,
                       std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return x + y; };
    if (binary_op<double, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, stack))
        return true;
    return false;
}

bool Subtraction::execute(const ValueType& a, const ValueType& b, std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return x - y; };
    if (binary_op<double, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, stack))
        return true;
    return false;
}

void Constructor::operator()(std::vector<ValueType>& stack) const
{
    if (stack.size() < num_args_)
    {
        XYZ_THROW("Not enough values on stack for " + name + ". Stack size: "
            + std::to_string(stack.size())
            + ", required: " + std::to_string(num_args_) + ".");
    }

    std::vector<double> values;
    values.reserve(num_args_);
    for (size_t i = stack.size() - num_args_; i < stack.size(); i++)
    {
        if (!std::holds_alternative<double>(stack[i]))
        {
            XYZ_THROW("Expected a double on the stack at index "
                + std::to_string(stack.size() - i - 1) + " for " + name + ".");
        }
        values.push_back(std::get<double>(stack[i]));
    }
    stack.resize(stack.size() - num_args_);

    execute(values, stack);
    print(stack.back());
}

void Vector3::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::Vector3D(values[0], values[1], values[2]));
}

bool Multiplication::execute(const ValueType& a,
                             const ValueType& b,
                             std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return x * y; };
    if (binary_op<double, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, double>(a, b, op, stack))
        return true;
    if (binary_op<double, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, double>(a, b, op, stack))
        return true;
    if (binary_op<double, Xyz::Vector4D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    return false;
}

bool Division::execute(const ValueType& a,
                       const ValueType& b,
                       std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return x / y; };
    if (binary_op<double, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix3D, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Matrix4D, double>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    return false;
}

bool Power::execute(const ValueType& a,
                    const ValueType& b,
                    std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return std::pow(x, y); };
    return binary_op<double, double>(a, b, op, stack);
}

bool Arctangent2::execute(const ValueType& a,
                          const ValueType& b,
                          std::vector<ValueType>& stack) const
{
    auto op = [](auto&& y, auto&& x) { return std::atan2(y, x); };
    return binary_op<double, double>(a, b, op, stack);
}

bool InnerProduct::execute(const ValueType& a,
                           const ValueType& b,
                           std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return Xyz::dot(x, y); };
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, stack))
        return true;
    return false;
}

bool CrossProduct::execute(const ValueType& a,
                           const ValueType& b,
                           std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x, auto&& y) { return Xyz::cross(x, y); };
    return binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, stack);
}

bool Negate::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x) { return -x; };
    if (unary_op<double>(a, op, stack))
        return true;
    if (unary_op<Xyz::Vector3D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Vector4D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Matrix3D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Matrix4D>(a, op, stack))
        return true;
    return false;
}

bool SquareRoot::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::sqrt(x); }, stack);
}

bool Sine::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::sin(x); }, stack);
}

bool Cosine::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::cos(x); }, stack);
}

bool Tangent::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::tan(x); }, stack);
}

bool Arcsine::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::asin(x); }, stack);
}

bool Arccosine::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::acos(x); }, stack);
}

bool Arctangent::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::atan(x); }, stack);
}

bool Logarithm::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    return unary_op<double>(a, [](auto&& x) { return std::log(x); }, stack);
}

bool Transpose::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x) { return Xyz::transpose(x); };
    if (unary_op<Xyz::Matrix3D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Matrix4D>(a, op, stack))
        return true;
    return false;
}

bool Invert::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x) { return Xyz::invert(x); };
    if (unary_op<Xyz::Matrix3D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Matrix4D>(a, op, stack))
        return true;
    return false;
}

bool Normalize::execute(const ValueType& a, std::vector<ValueType>& stack) const
{
    auto op = [](auto&& x) { return Xyz::normalize(x); };
    if (unary_op<Xyz::Vector3D>(a, op, stack))
        return true;
    if (unary_op<Xyz::Vector4D>(a, op, stack))
        return true;
    return false;
}

void Vector4::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::Vector4D(values[0], values[1], values[2], values[3]));
}

void Identity3::execute(const std::vector<double>&, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::Matrix3D::identity());
}

void Identity4::execute(const std::vector<double>&, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::Matrix4D::identity());
}

void Rotate2::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::rotate2(values[0]));
}

void RotateX::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::rotate_x(values[0]));
}

void RotateY::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::rotate_y(values[0]));
}

void RotateZ::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::rotate_z(values[0]));
}

void Translate2::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::translate2(Xyz::Vector2D(values[0], values[1])));
}

void Translate3::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::translate3(Xyz::Vector3D(values[0], values[1], values[2])));
}

void Scale2::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::scale2(Xyz::Vector2D(values[0], values[1])));
}

void Scale3::execute(const std::vector<double>& values, std::vector<ValueType>& stack) const
{
    stack.emplace_back(Xyz::affine::scale3(Xyz::Vector3D(values[0], values[1], values[2])));
}

void Matrix3::operator()(std::vector<ValueType>& stack) const
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for " + name + ".");

    // Takes the upper left 3x3 part of a 4x4 matrix, leaves a 3x3 matrix
    // as it is, and otherwise makes a matrix from the top nine numbers.
    if (std::holds_alternative<Xyz::Matrix4D>(stack.back()))
    {
        const auto m4 = std::get<Xyz::Matrix4D>(stack.back());
        stack.pop_back();
        stack.emplace_back(Xyz::make_submatrix<3, 3>(m4));
    }
    else if (!std::holds_alternative<Xyz::Matrix3D>(stack.back()))
    {
        if (stack.size() < 9)
        {
            XYZ_THROW("Not enough values on stack for " + name + ". Stack size: "
                + std::to_string(stack.size()) + ", required: 9.");
        }

        std::vector<double> values;
        values.reserve(9);
        for (size_t i = stack.size() - 9; i < stack.size(); i++)
        {
            if (!std::holds_alternative<double>(stack[i]))
            {
                XYZ_THROW("Expected a double on the stack at index "
                    + std::to_string(stack.size() - i - 1) + " for " + name + ".");
            }
            values.push_back(std::get<double>(stack[i]));
        }
        stack.resize(stack.size() - 9);
        stack.emplace_back(Xyz::Matrix3D(values[0], values[1], values[2],
                                         values[3], values[4], values[5],
                                         values[6], values[7], values[8]));
    }

    print(stack.back());
}

void Duplicate::operator()(std::vector<ValueType>& stack) const
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for " + name + ".");

    stack.push_back(stack.back());
    print(stack.back());
}

void Swap::operator()(std::vector<ValueType>& stack) const
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for " + name + ".");

    std::swap(stack[stack.size() - 2], stack.back());
    print(stack.back());
}

void Rotate::operator()(std::vector<ValueType>& stack) const
{
    if (stack.size() < 3)
        XYZ_THROW("Not enough values on stack for " + name + ".");

    // Moves the third value from the top to the top: a b c -> b c a
    std::rotate(stack.end() - 3, stack.end() - 2, stack.end());
    print(stack.back());
}

void Pop::operator()(std::vector<ValueType>& stack) const
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for " + name + ".");

    stack.pop_back();
}

void Clear::operator()(std::vector<ValueType>& stack) const
{
    stack.clear();
}

void PrintStack::operator()(std::vector<ValueType>& stack) const
{
    for (const auto& value : stack)
        print(value);
}

void Help::operator()(std::vector<ValueType>&) const
{
    std::vector<std::string_view> names;
    names.reserve(commands_.size());
    for (const auto& [command_name, _] : commands_)
        names.push_back(command_name);
    std::ranges::sort(names);

    std::cout << "Commands:";
    size_t column = 9;
    for (const auto& command_name : names)
    {
        if (column + 1 + command_name.size() > 78)
        {
            std::cout << "\n   ";
            column = 3;
        }
        std::cout << ' ' << command_name;
        column += 1 + command_name.size();
    }
    std::cout << "\n";
}
