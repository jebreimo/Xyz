##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-18.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 2.8)

set(PARENT_PROJECT Xyz)
project(${PARENT_PROJECT}Test)

set(SOURCE_FILES
    main.cpp
    Fundamentals/test_Matrix.cpp
    Fundamentals/test_Vector.cpp
    ../Xyz/Vector.hpp)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
#    ${PARENT_PROJECT}
    Ytest)

add_test(${PARENT_PROJECT} ${PROJECT_NAME})
