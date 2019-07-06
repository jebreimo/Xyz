##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.13)
project(Xyz)

set(CMAKE_CXX_STANDARD 14)

add_library(Xyz STATIC
    include/Xyz/Matrix.hpp
    include/Xyz/Vector.hpp
    include/Xyz/Xyz.hpp
    include/Xyz/Fundamentals/MatrixClass.hpp
    include/Xyz/Fundamentals/MatrixFunctions.hpp
    include/Xyz/Fundamentals/MatrixOperators.hpp
    include/Xyz/Fundamentals/Projections.hpp
    include/Xyz/Fundamentals/Transformations.hpp
    include/Xyz/Fundamentals/VectorClass.hpp
    include/Xyz/Fundamentals/VectorFunctions.hpp
    include/Xyz/Fundamentals/VectorOperators.hpp
    include/Xyz/Geometry/Line.hpp
    include/Xyz/Geometry/LineClipping.hpp
    include/Xyz/Geometry/LineLineIntersection.hpp
    include/Xyz/Geometry/LineSegment.hpp
    include/Xyz/Geometry/Rectangle.hpp
    include/Xyz/Geometry/Triangle.hpp
    include/Xyz/Utilities/Clamp.hpp
    include/Xyz/Utilities/Comparisons.hpp
    include/Xyz/Utilities/Constants.hpp
    include/Xyz/Utilities/RandomGenerator.hpp
    include/Xyz/Utilities/Utilities.hpp
    src/Xyz/Utilities/RandomGenerator.cpp
    )

target_include_directories(Xyz
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Xyz::Xyz ALIAS Xyz)

if (EXISTS "${CMAKE_SOURCE_DIR}/external/Ytest/CMakeLists.txt")
    add_subdirectory(external/Ytest EXCLUDE_FROM_ALL)
    add_subdirectory(tests)

    enable_testing()
    add_test(XyzTest
        COMMAND XyzTest)
endif ()
