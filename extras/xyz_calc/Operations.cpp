//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-02-01.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Operations.hpp"
#include <cmath>
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


bool division(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for division.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<double>(a) / std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) / std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) / std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix3D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix3D>(a) / std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Matrix4D>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::get<Xyz::Matrix4D>(a) / std::get<double>(b));
    }
    else if (std::holds_alternative<Xyz::Vector3D>(a) && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector3D>(a) / std::get<Xyz::Vector3D>(b));
    }
    else if (std::holds_alternative<Xyz::Vector4D>(a) && std::holds_alternative<Xyz::Vector4D>(b))
    {
        stack.emplace_back(std::get<Xyz::Vector4D>(a) / std::get<Xyz::Vector4D>(b));
    }
    else
    {
        XYZ_THROW("Incompatible types for division.");
    }
    return true;
}

bool square_root(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for square root.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::sqrt(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for square root.");
    }
    return true;
}

bool power(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for power.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::pow(std::get<double>(a), std::get<double>(b)));
    }
    else
    {
        XYZ_THROW("Incompatible types for power.");
    }
    return true;
}

bool sine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for sine.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::sin(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for sine.");
    }
    return true;
}

bool cosine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for cosine.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::cos(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for cosine.");
    }
    return true;
}

bool tangent(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for tangent.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::tan(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for tangent.");
    }
    return true;
}

bool arcsine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arcsine.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::asin(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for arcsine.");
    }
    return true;
}

bool arccosine(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arccosine.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::acos(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for arccosine.");
    }
    return true;
}

bool arctangent(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for arctangent.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::atan(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for arctangent.");
    }
    return true;
}

bool arctangent2(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for arctangent2.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b))
    {
        stack.emplace_back(std::atan2(std::get<double>(a), std::get<double>(b)));
    }
    else
    {
        XYZ_THROW("Incompatible types for arctangent2.");
    }
    return true;
}

bool logarithm(std::vector<ValueType>& stack)
{
    if (stack.empty())
        XYZ_THROW("Not enough values on stack for logarithm.");

    const auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<double>(a))
    {
        stack.emplace_back(std::log(std::get<double>(a)));
    }
    else
    {
        XYZ_THROW("Incompatible type for logarithm.");
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

bool cross_product(std::vector<ValueType>& stack)
{
    if (stack.size() < 2)
        XYZ_THROW("Not enough values on stack for cross product.");

    auto b = stack.back();
    stack.pop_back();
    auto a = stack.back();
    stack.pop_back();

    if (std::holds_alternative<Xyz::Vector3D>(a) && std::holds_alternative<Xyz::Vector3D>(b))
    {
        stack.emplace_back(Xyz::cross(std::get<Xyz::Vector3D>(a),
                                      std::get<Xyz::Vector3D>(b)));
    }
    else
    {
        XYZ_THROW("Incompatible types for cross product.");
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

double clean(double value)
{
    if (value == -0.0)
        value = 0.0;

    const auto whole = std::round(value);
    if (std::fabs(value - whole) < 1e-10)
        return whole;
    return value;
}

template <typename T, unsigned N>
Xyz::Vector<T, N> clean(const Xyz::Vector<T, N>& vector)
{
    Xyz::Vector<T, N> result;
    for (unsigned i = 0; i < N; ++i)
    {
        result.values[i] = clean(vector.values[i]);
    }
    return result;
}

template <typename T, unsigned M, unsigned N>
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

bool print(std::ostream& stream, const ValueType& value)
{
    std::visit([&stream](const auto& v)
    {
        stream << clean(v);
    }, value);
    return true;
}
