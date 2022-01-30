##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-18.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.13)

include(FetchContent)
FetchContent_Declare(ytest
    GIT_REPOSITORY "https://github.com/jebreimo/Ytest.git"
    GIT_TAG v0.1.0)
FetchContent_MakeAvailable(ytest)

add_executable(XyzTest
    XyzTestMain.cpp
    YtestUtilities.hpp
    test_InvertMatrix.cpp
    test_Matrix.cpp
    test_MatrixDeterminant.cpp
    test_Projections.cpp
    test_Transforms.cpp
    test_Vector.cpp
    test_CoordinateSystem.cpp
    test_LineClipping.cpp
    test_Intersections.cpp
    test_Triangle.cpp)

target_link_libraries(XyzTest
    Xyz::Xyz
    Ytest::Ytest)
