//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-13.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "NewOperations.hpp"

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
