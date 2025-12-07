//****************************************************************************
// Copyright © 2015 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2015-12-18.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include "RotationMatrix.hpp"

namespace Xyz
{
    /**
     * @brief Contains functions for creating affine (homogeneous)
     *  transformation matrices.
     *
     * These functions create matrices that can be used to transform points
     * in 2D or 3D space. The transformations include scaling, *translation*,
     * and rotation.
     */
    namespace affine
    {
        /**
         * @brief Returns the homogeneous scaling matrix for 2D points.
         */
        template <typename T>
        Matrix<T, 3, 3> scale2(T sx, T sy)
        {
            return Matrix<T, 3, 3>{
                sx, 0, 0,
                0, sy, 0,
                0, 0, 1
            };
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 2D points.
         */
        template <typename T>
        Matrix<T, 3, 3> scale2(const Vector<T, 2>& scales)
        {
            return scale2(scales[0], scales[1]);
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 3D points.
         * @param sx The scale factor in the x direction.
         * @param sy The scale factor in the y direction.
         * @param sz The scale factor in the z direction.
         */
        template <typename T>
        Matrix<T, 4, 4> scale3(T sx, T sy, T sz)
        {
            return Matrix<T, 4, 4>{
                sx, 0, 0, 0,
                0, sy, 0, 0,
                0, 0, sz, 0,
                0, 0, 0, 1
            };
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 3D points.
         * @param scales The scale factors in the x, y, and z directions.
         */
        template <typename T>
        Matrix<T, 4, 4> scale3(const Vector<T, 3>& scales)
        {
            return scale3(scales[0], scales[1], scales[2]);
        }

        template <typename T>
        Matrix<T, 3, 3> translate2(T x, T y)
        {
            return Matrix<T, 3, 3>{
                1, 0, x,
                0, 1, y,
                0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 3, 3> translate2(const Vector<T, 2>& offsets)
        {
            return translate2(offsets[0], offsets[1]);
        }

        template <typename T>
        Matrix<T, 4, 4> translate3(T x, T y, T z)
        {
            return {
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> translate3(const Vector<T, 3>& offsets)
        {
            return translate3(offsets[0], offsets[1], offsets[2]);
        }

        template <typename T>
        Matrix<T, 3, 3> transposed_translate3(T x, T y)
        {
            return Matrix<T, 3, 3>{
                1, 0, 0,
                0, 1, 0,
                x, y, 1
            };
        }

        template <typename T>
        Matrix<T, 3, 3> transposed_translate3(const Vector<T, 2>& offsets)
        {
            return transpose_translate(offsets[0], offsets[1]);
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_translate4(T x, T y, T z)
        {
            return Matrix<T, 4, 4>{
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1
            };
        }

        template <typename T>
        Matrix<T, 4, 4> transposed_translate4(const Vector<T, 3>& offsets)
        {
            return transposed_translate4(offsets[0], offsets[1], offsets[2]);
        }
    }

    /** @brief Contains functions for creating linear (non-homogeneous)
     *  transformation matrices.
     *
     * These functions create matrices that can be used to transform points
     * in 2D or 3D space. The transformations include scaling and rotation,
     * but not translation.
     */
    namespace linear
    {
        /**
         * @brief Returns the homogeneous scaling matrix for 2D points.
         */
        template <typename T>
        Matrix<T, 2, 2> scale2(T sx, T sy)
        {
            return {
                sx, 0,
                0, sy
            };
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 2D points.
         */
        template <typename T>
        Matrix<T, 2, 2> scale2(const Vector<T, 2>& scales)
        {
            return scale2(scales[0], scales[1]);
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 3D points.
         * @param sx The scale factor in the x direction.
         * @param sy The scale factor in the y direction.
         * @param sz The scale factor in the z direction.
         */
        template <typename T>
        Matrix<T, 3, 3> scale3(T sx, T sy, T sz)
        {
            return {
                sx, 0, 0,
                0, sy, 0,
                0, 0, sz
            };
        }

        /**
         * @brief Returns the homogeneous scaling matrix for 3D points.
         * @param scales The scale factors in the x, y, and z directions.
         */
        template <typename T>
        Matrix<T, 3, 3> scale3(const Vector<T, 3>& scales)
        {
            return scale3(scales[0], scales[1], scales[2]);
        }
    }
}
