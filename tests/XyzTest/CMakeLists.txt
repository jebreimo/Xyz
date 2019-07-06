##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-18.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

add_executable(XyzTest
    main.cpp
    Fundamentals/YtestUtilities.hpp
    Fundamentals/test_Matrix.cpp
    Fundamentals/test_Matrix.cpp
    Fundamentals/test_Projections.cpp
    Fundamentals/test_Transforms.cpp
    Fundamentals/test_Vector.cpp
    #    Geometry/test_Distance.cpp
    Geometry/test_Intersections.cpp
    Geometry/test_Triangle.cpp
    Geometry/test_LineClipping.cpp.cpp
    )

target_link_libraries(XyzTest
    Xyz::Xyz
    Ytest::Ytest)
