//****************************************************************************
// Copyright © 2026 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2026-11-13.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <utility>
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