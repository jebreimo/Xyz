//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-02-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Operations.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

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
    void print_op(const ValueT1& a, const ValueT2& b, const ResultT& result, const std::string& op_name)
    {
        std::cout << clean(a) << " " << op_name << " " << clean(b) << " = " << clean(result) << std::endl;
    }

    template <typename ValueT1, typename ResultT>
    void print_op(const ValueT1& a, const ResultT& result, const std::string& op_name)
    {
        std::cout << op_name << " " << clean(a) << " = " << clean(result) << std::endl;
    }

    template <typename T, typename U, typename Operation>
    bool binary_op(const ValueType& a,
                   const ValueType& b,
                   const Operation& op,
                   const std::string& op_name,
                   std::vector<ValueType>& stack)
    {
        if (std::holds_alternative<T>(a) && std::holds_alternative<U>(b))
        {
            const auto& lhs = std::get<T>(a);
            const auto& rhs = std::get<U>(b);
            auto result = op(lhs, rhs);
            print_op(lhs, rhs, result, op_name);
            stack.emplace_back(result);
            return true;
        }
        return false;
    }

    template <typename T, typename Operation>
    bool unary_op(const ValueType& a,
                  const Operation& op,
                  const std::string& op_name,
                  std::vector<ValueType>& stack)
    {
        if (std::holds_alternative<T>(a))
        {
            const auto& value = std::get<T>(a);
            auto result = op(value);
            print_op(value, result, op_name);
            stack.emplace_back(result);
            return true;
        }
        return false;
    }
}

bool addition(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for addition.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return x + y; };

    if (binary_op<double, double>(a, b, op, "+", stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "+", stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, "+", stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, "+", stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, "+", stack))
        return true;
    XYZ_THROW("Incompatible types for addition.");
}

bool subtraction(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for subtraction.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return x - y; };

    if (binary_op<double, double>(a, b, op, "-", stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "-", stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, "-", stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, "-", stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, "-", stack))
        return true;

    XYZ_THROW("Incompatible types for subtraction.");
}

bool multiplication(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for multiplication.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return x * y; };

    if (binary_op<double, double>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Vector3D, double>(a, b, op, "*", stack))
        return true;
    if (binary_op<double, Xyz::Vector3D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Vector4D, double>(a, b, op, "*", stack))
        return true;
    if (binary_op<double, Xyz::Vector4D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Matrix3D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Matrix4D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Matrix3D, Xyz::Vector3D>(a, b, op, "*", stack))
        return true;
    if (binary_op<Xyz::Matrix4D, Xyz::Vector4D>(a, b, op, "*", stack))
        return true;
    XYZ_THROW("Incompatible types for multiplication.");
}


bool division(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for division.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return x / y; };

    if (binary_op<double, double>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Vector3D, double>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Vector4D, double>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Matrix3D, double>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Matrix4D, double>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "/", stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, "/", stack))
        return true;
    XYZ_THROW("Incompatible types for division.");
}

bool negate(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for negate.");

    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x) { return -x; };

    if (unary_op<double>(a, op, "neg", stack))
        return true;

    if (unary_op<Xyz::Vector3D>(a, op, "neg", stack))
        return true;
    if (unary_op<Xyz::Vector4D>(a, op, "neg", stack))
        return true;
    if (unary_op<Xyz::Matrix3D>(a, op, "neg", stack))
        return true;
    if (unary_op<Xyz::Matrix4D>(a, op, "neg", stack))
        return true;
    XYZ_THROW("Incompatible type for negate.");
}

bool square_root(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for square root.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::sqrt(x); }, "sqrt", stack))
        return true;

    XYZ_THROW("Incompatible type for square root.");
}

bool power(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for power.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    if (binary_op<double, double>(a, b, [](auto&& x, auto&& y) { return std::pow(x, y); }, "pow", stack))
        return true;
    XYZ_THROW("Incompatible types for power.");
}

bool sine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for sine.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::sin(x); }, "sin", stack))
        return true;

    XYZ_THROW("Incompatible type for sine.");
}

bool cosine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for cosine.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::cos(x); }, "cos", stack))
        return true;

    XYZ_THROW("Incompatible type for cosine.");
}

bool tangent(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for tangent.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::tan(x); }, "tan", stack))
        return true;

    XYZ_THROW("Incompatible type for tangent.");
}

bool arcsine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arcsine.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::asin(x); }, "asin", stack))
        return true;

    XYZ_THROW("Incompatible type for arcsine.");
}

bool arccosine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arccosine.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::acos(x); }, "acos", stack))
        return true;

    XYZ_THROW("Incompatible type for arccosine.");
}

bool arctangent(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arctangent.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::atan(x); }, "atan", stack))
        return true;

    XYZ_THROW("Incompatible type for arctangent.");
}

bool arctangent2(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for arctangent2.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    if (binary_op<double, double>(a, b, [](auto&& y, auto&& x) { return std::atan2(y, x); }, "atan2", stack))
        return true;

    XYZ_THROW("Incompatible types for arctangent2.");
}

bool logarithm(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for logarithm.");

    const auto a = stack.back();
    stack.pop_back();

    if (unary_op<double>(a, [](auto&& x) { return std::log(x); }, "log", stack))
        return true;

    XYZ_THROW("Incompatible type for logarithm.");
}

bool inner_product(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for inner product.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return Xyz::dot(x, y); };

    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "dot", stack))
        return true;
    if (binary_op<Xyz::Vector4D, Xyz::Vector4D>(a, b, op, "dot", stack))
        return true;
    XYZ_THROW("Incompatible types for inner product.");
}

bool cross_product(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for cross product.");

    const auto b = stack.back();
    stack.pop_back();
    const auto a = stack.back();
    stack.pop_back();

    auto op = [](auto&& x, auto&& y) { return Xyz::cross(x, y); };

    if (binary_op<Xyz::Vector3D, Xyz::Vector3D>(a, b, op, "cross", stack))
        return true;
    XYZ_THROW("Incompatible types for cross product.");
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

bool normalize(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for normalize.");

    const auto value = stack.back();
    stack.pop_back();

    if (std::holds_alternative<Xyz::Vector3D>(value))
    {
        stack.emplace_back(Xyz::normalize(std::get<Xyz::Vector3D>(value)));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(value))
    {
        stack.emplace_back(Xyz::normalize(std::get<Xyz::Vector4D>(value)));
    }
    else
    {
        XYZ_THROW("Incompatible type for normalize.");
    }
    return true;
}

bool matrix3(std::vector<ValueType>& stack)
{
    if (stack.empty())
    {
        XYZ_THROW("Not enough values on stack for matrix3.");
    }

    if (std::holds_alternative<Xyz::Matrix4D>(stack.back()))
    {
        const auto m4 = std::get<Xyz::Matrix4D>(stack.back());
        stack.pop_back();
        Xyz::Matrix3D m3 = Xyz::make_submatrix<3, 3>(m4);
        stack.emplace_back(m3);
    }
    else if (!std::holds_alternative<Xyz::Matrix3D>(stack.back()))
    {
        const auto values = get_doubles(stack, 9);
        stack.emplace_back(Xyz::Matrix3D(values[0], values[1], values[2],
                                         values[3], values[4], values[5],
                                         values[6], values[7], values[8]));
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

bool swap(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for swap.");

    std::swap(stack[stack.size() - 2], stack.back());
    return true;
}

bool rotate(std::vector<ValueType>& stack)
{
    if (stack.size() < 3)
        XYZ_THROW("Not enough values on stack for rotate.");

    // Moves the third value from the top to the top: a b c -> b c a
    std::rotate(stack.end() - 3, stack.end() - 2, stack.end());
    return true;
}

bool pop(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for pop.");

    stack.pop_back();
    return false;
}

bool clear(std::vector<ValueType>& stack)
{
    stack.clear();
    return false;
}

bool print_stack(std::vector<ValueType>& stack)
{
    for (const auto& value : stack)
    {
        print(std::cout, value);
        std::cout << "\n";
    }
    return false;
}

bool print(std::ostream& stream, const ValueType& value)
{
    std::visit([&stream](const auto& v)
    {
        stream << clean(v);
    }, value);
    return true;
}
