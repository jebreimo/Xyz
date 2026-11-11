# ===========================================================================
# Copyright © 2026 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2026-01-31.
#
# This file is distributed under the Zero-Clause BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.18)

include(FetchContent)
# SOURCE_SUBDIR points to a directory without a CMakeLists.txt to prevent
# isocline's own build (shared library, examples, install rules) from being
# added. The static library is built from its single source file below.
FetchContent_Declare(isocline
    GIT_REPOSITORY https://github.com/daanx/isocline.git
    GIT_TAG v1.1.0
    SOURCE_SUBDIR do-not-build
)
FetchContent_MakeAvailable(isocline)

add_library(isocline STATIC
    ${isocline_SOURCE_DIR}/src/isocline.c
)

target_include_directories(isocline
    PUBLIC
        ${isocline_SOURCE_DIR}/include
)

add_executable(xyz_calc
    XyzCalc.cpp
    Operations.cpp
    Operations.hpp
    ParserTools/StringTokenizer.hpp
)

target_link_libraries(xyz_calc
    Xyz::Xyz
    isocline
)
