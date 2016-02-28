//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 13.02.2016
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Xyz.hpp"
#include "../../Submodules/Ytest/Ytest.hpp"

namespace {
    using namespace Xyz;

//static void test_ContainsPoint()
//{
//    LineSegment<double, 2> ls(vector2(0.0, 0.0), vector2(4.0, 6.0));
//    JT_ASSERT(containsPoint(ls, vector2(0.0, 0.0), 1e-9));
//    JT_ASSERT(containsPoint(ls, vector2(4.0, 6.0), 1e-9));
//    JT_ASSERT(containsPoint(ls, vector2(-1e-10, 0.0), 1e-9));
//    JT_ASSERT(containsPoint(ls, vector2(4 + 1e-10, 6.0), 1e-9));
//    JT_ASSERT(!containsPoint(ls, vector2(-1e-8, 0.0), 1e-9));
//    JT_ASSERT(!containsPoint(ls, vector2(4 + 1e-8, 6.0), 1e-9));
//    JT_ASSERT(containsPoint(ls, vector2(1.4 / 2.1, 1.0), 1e-9));
//}
//
//static void test_Distance()
//{
//    auto l = makeLine(vector2(-3.0, 2.0), vector2(6.0, 0.0));
//    auto p = vector2(0.0, 0.0);
//    JT_EQUIVALENT(getDistance(l, p), -2, 1e-10);
//}

    template <typename T>
    Xyz::LineSegment<T, 2> makeLineSegment(T x1, T y1, T x2, T y2)
    {
        return Xyz::makeLineSegment(makeVector(x1, y1), makeVector(x2, y2));
    }

    static void test_getIntersectionFactors()
    {
        auto a = makeLineSegment<int>(-4, -2, 4, 2);
        auto b = makeLineSegment<int>(3, -9, -1, 3);
        auto intersection = getIntersectionPositions(a, b, 1e-10);
        Y_EQUAL(std::get<0>(intersection), LineRelationship::INTERSECTING);
        Y_EQUAL(std::get<1>(intersection), 0.5);
        Y_EQUAL(std::get<2>(intersection), 0.75);
    }

//static void test_Overlap()
//{
//    auto a = makeLineSegment(vector2(0.0, 0.0), vector2(5.0, 5.0));
//    auto b = makeLineSegment(vector2(6.0, 6.0), vector2(2.0, 2.0));
//    std::pair<double, double> offsA, offsB;
//    JT_ASSERT(overlap(offsA, offsB, a, b, 1e-9));
//    JT_EQUAL(offsA.first, 1.0);
//    JT_EQUAL(offsA.second, 0.4);
//    JT_EQUAL(offsB.first, 0.25);
//    JT_EQUAL(offsB.second, 1.0);
//}
//
//static void test_AreTangentialAtPoint()
//{
//    Vector<double, 2> p[] = {
//        vector2(0.0, 0.0),
//        vector2(-1.0, 3.0),
//        vector2(3.0, 1.0),
//        vector2(1.0, -3.0),
//        vector2(-3.0, -1.0),
//        vector2(-1.9, 5.7),
//        vector2(5.7, 1.9)
//    };
//
//    JT_ASSERT(areTangentialAtPoint(p[0], p[1], p[2], p[3], p[4]));
//    JT_ASSERT(areTangentialAtPoint(p[0], p[1], p[2], p[4], p[3]));
//    JT_ASSERT(areTangentialAtPoint(p[0], p[2], p[1], p[3], p[4]));
//    JT_ASSERT(areTangentialAtPoint(p[0], p[2], p[1], p[4], p[3]));
//    JT_ASSERT(!areTangentialAtPoint(p[0], p[1], p[3], p[2], p[4]));
//    JT_ASSERT(!areTangentialAtPoint(p[0], p[1], p[3], p[4], p[2]));
//    JT_ASSERT(!areTangentialAtPoint(p[0], p[3], p[1], p[2], p[4]));
//    JT_ASSERT(!areTangentialAtPoint(p[0], p[3], p[1], p[4], p[2]));
//    JT_ASSERT(areTangentialAtPoint(p[0], p[1], p[2], p[3], p[5], 1e-9));
//}

    Y_SUBTEST("Geometry",
//          test_ContainsPoint,
//          test_Distance,
              test_getIntersectionFactors,
//          test_Overlap,
//          test_AreTangentialAtPoint
    );

}
