##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.16)

project(Xyz VERSION 0.2.85)

set(CMAKE_CXX_STANDARD 17)

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

# Install option
option(XYZ_INSTALL "Generate the install target" ${XYZ_MASTER_PROJECT})

add_library(Xyz STATIC
    include/Xyz/Clamp.hpp
    include/Xyz/ComplexApprox.hpp
    include/Xyz/Constants.hpp
    include/Xyz/CoordinateSystem.hpp
    include/Xyz/Face.hpp
    include/Xyz/FloatType.hpp
    include/Xyz/HalfEdge.hpp
    include/Xyz/InvertMatrix.hpp
    include/Xyz/Line.hpp
    include/Xyz/LineClipping.hpp
    include/Xyz/LineLineIntersection.hpp
    include/Xyz/LineSegment.hpp
    include/Xyz/Matrix.hpp
    include/Xyz/MatrixDeterminant.hpp
    include/Xyz/MatrixTransformations.hpp
    include/Xyz/Mesh.hpp
    include/Xyz/ProjectionMatrix.hpp
    include/Xyz/QuadraticEquation.hpp
    include/Xyz/RandomGenerator.hpp
    include/Xyz/Rectangle.hpp
    include/Xyz/SphericalPoint.hpp
    include/Xyz/Triangle.hpp
    include/Xyz/Utilities.hpp
    include/Xyz/Vector.hpp
    include/Xyz/Xyz.hpp
    include/Xyz/XyzException.hpp
    src/Xyz/Face.cpp
    src/Xyz/RandomGenerator.cpp
    )

include(GNUInstallDirs)

target_include_directories(Xyz
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

add_library(Xyz::Xyz ALIAS Xyz)

##
## Unit tests
##

if(XYZ_BUILD_TEST)
    enable_testing()
    add_subdirectory(tests/XyzTest)
    add_subdirectory(tests/CatchXyzTest)
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
        DIRECTORY
            include/
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}
        )

endif()

if (DEFINED LOCAL_SOURCE_PACKAGE_DIR OR DEFINED ENV{LOCAL_SOURCE_PACKAGE_DIR})
    if (NOT LOCAL_SOURCE_PACKAGE_DIR)
        set(LOCAL_SOURCE_PACKAGE_DIR $ENV{LOCAL_SOURCE_PACKAGE_DIR})
    endif()
    if (XYZ_MASTER_PROJECT)
        find_package(Git REQUIRED)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE CURRENT_GIT_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        add_custom_target(${PROJECT_NAME}_BUILD_SRC_TARBALL
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND ${GIT_EXECUTABLE} archive -v --format=tar.gz --prefix=${PROJECT_NAME}/ HEAD > ${LOCAL_SOURCE_PACKAGE_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}.${CURRENT_GIT_REVISION}.tar.gz
            COMMAND echo Created ${LOCAL_SOURCE_PACKAGE_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}.${CURRENT_GIT_REVISION}.tar.gz
            )
    endif()
endif ()
