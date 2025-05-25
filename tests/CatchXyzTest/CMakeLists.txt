##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-18.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.20)

include(FetchContent)
FetchContent_Declare(catch
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG "v3.5.3"
)
FetchContent_MakeAvailable(catch)

add_executable(CatchXyzTest
    test_Approx.cpp
    test_ComplexApprox.cpp
    test_CoordinateSystem.cpp
    test_Interpolation.cpp
    test_Intersections.cpp
    test_InvertMatrix.cpp
    test_MatrixDeterminant.cpp
    test_Mesh.cpp
    test_PlaneSegment.cpp
    test_Projections.cpp
    test_QuadraticEquation.cpp
    test_Rect.cpp
    test_Transforms.cpp
    test_Triangle.cpp
    test_Vector.cpp
)

target_link_libraries(CatchXyzTest
    Xyz::Xyz
    Catch2::Catch2WithMain
)

add_test(NAME CatchXyzTest COMMAND CatchXyzTest)
