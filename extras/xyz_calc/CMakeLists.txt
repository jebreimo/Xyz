# ===========================================================================
# Copyright © 2026 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2026-01-31.
#
# This file is distributed under the Zero-Clause BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.16)

add_executable(xyz_calc
    linenoise/linenoise.c
    XyzCalc.cpp
    Operations.cpp
    Operations.hpp
    ParserTools/StringTokenizer.hpp
)

target_link_libraries(xyz_calc
    Xyz::Xyz
)
