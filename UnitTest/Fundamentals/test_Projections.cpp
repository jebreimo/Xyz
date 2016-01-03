//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2016-01-02.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../../Xyz/Xyz.hpp"

#include "YtestUtilities.hpp"

namespace {

    void test_PerspectiveProjection()
    {
        auto m = Xyz::makeFrustum(-1, 1, -1, 1, 9, 11);
        auto v = Xyz::vector4(-1.0, -1.0, -9.0, 1.0);
        auto w = m * v;
        Y_EQUAL(w, Xyz::vector4(-9, -9, -9, 9));
    }

    Y_SUBTEST("Fundamentals",
              test_PerspectiveProjection
    );
}
