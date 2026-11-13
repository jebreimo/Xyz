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
#include <utility>
#include <vector>
#include "TypeDefs.hpp"

class Operation
{
public:
    std::string name;


    virtual ~Operation() = default;

    virtual void operator()(std::vector<ValueType>& stack) const = 0;

protected:
    Operation(std::string  name)
        : name(std::move(name))
    {}
};

class UnaryOperation : public Operation
{
public:
    void operator()(std::vector<ValueType>& stack) const override;

protected:
    using Operation::Operation;

    virtual bool execute(const ValueType& a, std::vector<ValueType>& stack) const = 0;

    template <typename T, typename Op>
    bool unary_op(const ValueType& a,
                  const Op& op,
                  std::vector<ValueType>& stack) const;
};

class BinaryOperation : public Operation
{
public:
    void operator()(std::vector<ValueType>& stack) const override;

protected:
    using Operation::Operation;

    virtual bool execute(const ValueType& a,
                         const ValueType& b,
                         std::vector<ValueType>& stack) const = 0;

    template <typename T, typename U, typename Op>
    bool binary_op(const ValueType& a,
                   const ValueType& b,
                   const Op& op,
                   std::vector<ValueType>& stack) const;
};

class Addition : public BinaryOperation
{
public:
    Addition() : BinaryOperation("+")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Subtraction : public BinaryOperation
{
public:
    Subtraction() : BinaryOperation("-")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Constructor : public Operation
{
public:
    void operator()(std::vector<ValueType>& stack) const override;

protected:
    Constructor(std::string name, size_t num_args)
        : Operation(std::move(name)),
          num_args_(num_args)
    {
    }

    virtual void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const = 0;
private:
    size_t num_args_;
};

class Vector3 : public Constructor
{
public:
    Vector3() : Constructor("vec3", 3)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Multiplication : public BinaryOperation
{
public:
    Multiplication() : BinaryOperation("*")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Division : public BinaryOperation
{
public:
    Division() : BinaryOperation("/")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Power : public BinaryOperation
{
public:
    Power() : BinaryOperation("pow")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Arctangent2 : public BinaryOperation
{
public:
    Arctangent2() : BinaryOperation("atan2")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class InnerProduct : public BinaryOperation
{
public:
    InnerProduct() : BinaryOperation("dot")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class CrossProduct : public BinaryOperation
{
public:
    CrossProduct() : BinaryOperation("cross")
    {
    }

protected:
    bool execute(const ValueType& a,
                 const ValueType& b,
                 std::vector<ValueType>& stack) const override;
};

class Negate : public UnaryOperation
{
public:
    Negate() : UnaryOperation("neg")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class SquareRoot : public UnaryOperation
{
public:
    SquareRoot() : UnaryOperation("sqrt")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Sine : public UnaryOperation
{
public:
    Sine() : UnaryOperation("sin")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Cosine : public UnaryOperation
{
public:
    Cosine() : UnaryOperation("cos")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Tangent : public UnaryOperation
{
public:
    Tangent() : UnaryOperation("tan")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Arcsine : public UnaryOperation
{
public:
    Arcsine() : UnaryOperation("asin")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Arccosine : public UnaryOperation
{
public:
    Arccosine() : UnaryOperation("acos")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Arctangent : public UnaryOperation
{
public:
    Arctangent() : UnaryOperation("atan")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Logarithm : public UnaryOperation
{
public:
    Logarithm() : UnaryOperation("log")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Transpose : public UnaryOperation
{
public:
    Transpose() : UnaryOperation("transpose")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Invert : public UnaryOperation
{
public:
    Invert() : UnaryOperation("inv")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Normalize : public UnaryOperation
{
public:
    Normalize() : UnaryOperation("norm")
    {
    }

protected:
    bool execute(const ValueType& a, std::vector<ValueType>& stack) const override;
};

class Vector4 : public Constructor
{
public:
    Vector4() : Constructor("vec4", 4)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Identity3 : public Constructor
{
public:
    Identity3() : Constructor("id3", 0)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Identity4 : public Constructor
{
public:
    Identity4() : Constructor("id4", 0)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Rotate2 : public Constructor
{
public:
    Rotate2() : Constructor("rot2", 1)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class RotateX : public Constructor
{
public:
    RotateX() : Constructor("rot_x", 1)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class RotateY : public Constructor
{
public:
    RotateY() : Constructor("rot_y", 1)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class RotateZ : public Constructor
{
public:
    RotateZ() : Constructor("rot_z", 1)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Translate2 : public Constructor
{
public:
    Translate2() : Constructor("trans2", 2)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Translate3 : public Constructor
{
public:
    Translate3() : Constructor("trans3", 3)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Scale2 : public Constructor
{
public:
    Scale2() : Constructor("scale2", 2)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Scale3 : public Constructor
{
public:
    Scale3() : Constructor("scale3", 3)
    {
    }

protected:
    void execute(const std::vector<double>& values, std::vector<ValueType>& stack) const override;
};

class Matrix3 : public Operation
{
public:
    Matrix3() : Operation("mat3")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Duplicate : public Operation
{
public:
    Duplicate() : Operation("dup")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Swap : public Operation
{
public:
    Swap() : Operation("swap")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Rotate : public Operation
{
public:
    Rotate() : Operation("rot")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Pop : public Operation
{
public:
    Pop() : Operation("pop")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Clear : public Operation
{
public:
    Clear() : Operation("clear")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class PrintStack : public Operation
{
public:
    PrintStack() : Operation("print")
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;
};

class Help : public Operation
{
public:
    explicit Help(const std::unordered_map<std::string, const Operation*>& commands)
        : Operation("help"),
          commands_(commands)
    {
    }

    void operator()(std::vector<ValueType>& stack) const override;

private:
    const std::unordered_map<std::string, const Operation*>& commands_;
};
