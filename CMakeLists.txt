##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.13)
project(Xyz)

set(CMAKE_CXX_STANDARD 17)

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
    include/Xyz/Utilities/XyzException.hpp include/Xyz/Geometry/CoordinateSystem.hpp include/Xyz/Utilities/FloatType.hpp include/Xyz/Math/MatrixDeterminant.hpp include/Xyz/Math/InvertMatrix.hpp include/Xyz/Generators/RegularPolygonGenerator.hpp include/Xyz/Generators/RectangleGenerator.hpp)

target_include_directories(Xyz BEFORE
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Xyz::Xyz ALIAS Xyz)


##
## Unit tests
##

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/Ytest/CMakeLists.txt")
    add_subdirectory(external/Ytest EXCLUDE_FROM_ALL)
endif ()

enable_testing()

add_subdirectory(tests/XyzTest)


##
## "Export" the current build tree and make it possible for other modules
## in the same build tree to locate it with find_package.
##

export(TARGETS Xyz
    NAMESPACE Xyz::
    FILE XyzConfig.cmake)


##
## Installation
##

include(GNUInstallDirs)

install(TARGETS Xyz
    EXPORT XyzConfig
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(EXPORT XyzConfig
    FILE
        XyzConfig.cmake
    NAMESPACE
        Xyz::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/Xyz
    )

install(
    DIRECTORY
        include/
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    )
