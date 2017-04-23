//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 18.12.2015
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "Fundamentals/MatrixClass.hpp"
#include "Fundamentals/MatrixFunctions.hpp"
#include "Fundamentals/MatrixOperators.hpp"

namespace Xyz
{
    using Matrix2i = Matrix<int, 2, 2>;
    using Matrix2f = Matrix<float, 2, 2>;
    using Matrix2d = Matrix<double, 2, 2>;

    using Matrix3i = Matrix<int, 3, 3>;
    using Matrix3f = Matrix<float, 3, 3>;
    using Matrix3d = Matrix<double, 3, 3>;

    using Matrix4i = Matrix<int, 4, 4>;
    using Matrix4f = Matrix<float, 4, 4>;
    using Matrix4d = Matrix<double, 4, 4>;
}
