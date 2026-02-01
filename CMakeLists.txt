##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.16)

project(Xyz
    VERSION 0.9.6
    DESCRIPTION "A C++ library for 3D geometry and linear algebra"
)

set(CMAKE_CXX_STANDARD 20)

# Set XYZ_MASTER_PROJECT to ON if Xyz is the root level project.
if (NOT DEFINED XYZ_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(XYZ_MASTER_PROJECT ON)
        message("XYZ version is ${Xyz_VERSION}")
    else ()
        set(XYZ_MASTER_PROJECT OFF)
    endif ()
endif ()

# Test option
option(XYZ_BUILD_TEST "Build tests" ${XYZ_MASTER_PROJECT})

# Extras option
option(XYZ_BUILD_EXTRAS "Build extras" ${XYZ_MASTER_PROJECT})

# Install option
option(XYZ_INSTALL "Generate the install target" ${XYZ_MASTER_PROJECT})

function(Xyz_enable_all_warnings target)
    target_compile_options(${target}
        PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall>
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:AppleClang>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endfunction()

configure_file(src/Xyz/XyzVersion.hpp.in XyzVersion.hpp @ONLY)

add_library(Xyz STATIC
    include/Xyz/Clamp.hpp
    include/Xyz/ComplexApprox.hpp
    include/Xyz/Constants.hpp
    include/Xyz/CoordinateSystem.hpp
    include/Xyz/Face.hpp
    include/Xyz/FloatType.hpp
    include/Xyz/HalfEdge.hpp
    include/Xyz/Interpolation.hpp
    include/Xyz/IntersectionType.hpp
    include/Xyz/InvertMatrix.hpp
    include/Xyz/Line.hpp
    include/Xyz/LineClipper.hpp
    include/Xyz/LineClipping.hpp
    include/Xyz/LineLineIntersection.hpp
    include/Xyz/LineSegment.hpp
    include/Xyz/Matrix.hpp
    include/Xyz/MatrixDeterminant.hpp
    include/Xyz/Mesh.hpp
    include/Xyz/Orientation.hpp
    include/Xyz/Pgram3.hpp
    include/Xyz/Placement.hpp
    include/Xyz/Plane.hpp
    include/Xyz/PlanePlaneIntersection.hpp
    include/Xyz/ProjectionMatrix.hpp
    include/Xyz/QuadraticEquation.hpp
    include/Xyz/RandomNumberGenerator.hpp
    include/Xyz/Rectangle.hpp
    include/Xyz/RotationMatrix.hpp
    include/Xyz/SphericalPoint.hpp
    include/Xyz/TransformationMatrix.hpp
    include/Xyz/Triangle.hpp
    include/Xyz/Utilities.hpp
    include/Xyz/Vector.hpp
    include/Xyz/Xyz.hpp
    include/Xyz/XyzException.hpp
    src/Xyz/Face.cpp
    src/Xyz/IntersectionType.cpp
    src/Xyz/RandomNumberGenerator.cpp
    include/Xyz/LuDecomposition.hpp
)

include(GNUInstallDirs)

target_include_directories(Xyz
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(Xyz::Xyz ALIAS Xyz)

Xyz_enable_all_warnings(Xyz)

##
## Unit tests
##

if(XYZ_BUILD_TEST)
    enable_testing()
    add_subdirectory(tests/CatchXyzTest)
endif()

##
## Extras
##

if(XYZ_BUILD_EXTRAS)
    add_subdirectory(extras)
endif()

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

if(XYZ_INSTALL)

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
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/XyzVersion.hpp
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/Xyz
    )

    install(
        DIRECTORY
            include/
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
        )

endif()
