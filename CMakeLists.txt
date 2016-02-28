##****************************************************************************
## Copyright © 2015 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2015-12-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 2.8)
project(Xyz)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(Submodules)

set(PUBLIC_HEADERS_XYZ
    Xyz/Matrix.hpp
    Xyz/Vector.hpp
    Xyz/Xyz.hpp
    )

set(PUBLIC_HEADERS_FUNDAMENTALS
    Xyz/Fundamentals/MatrixClass.hpp
    Xyz/Fundamentals/MatrixFunctions.hpp
    Xyz/Fundamentals/MatrixOperators.hpp
    Xyz/Fundamentals/Projections.hpp
    Xyz/Fundamentals/Transforms.hpp
    Xyz/Fundamentals/VectorClass.hpp
    Xyz/Fundamentals/VectorFunctions.hpp
    Xyz/Fundamentals/VectorOperators.hpp
    )

set(PUBLIC_HEADERS_GEOMETRY
    Xyz/Geometry/LineIntersections.hpp
    Xyz/Geometry/Line.hpp
    Xyz/Geometry/LineSegment.hpp
    Xyz/Geometry/Triangle.hpp
    )

set(PUBLIC_HEADERS_UTILITIES
    Xyz/Utilities/Clamp.hpp
    Xyz/Utilities/Constants.hpp
    Xyz/Utilities/Utilities.hpp
    )

set(PUBLIC_HEADERS
    ${PUBLIC_HEADERS_XYZ}
    ${PUBLIC_HEADERS_FUNDAMENTALS}
    ${PUBLIC_HEADERS_GEOMETRY}
    ${PUBLIC_HEADERS_UTILITIES}
    )

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE ${PUBLIC_HEADERS})

if (HAS_YTEST)
    set(CTEST_OUTPUT_ON_FAILURE 1)
    add_subdirectory(UnitTest)
endif (HAS_YTEST)

install(FILES ${PUBLIC_HEADERS_XYZ}
    DESTINATION include/${PROJECT_NAME})
install(FILES ${PUBLIC_HEADERS_FUNDAMENTALS}
    DESTINATION include/${PROJECT_NAME}/Fundamentals)
install(FILES ${PUBLIC_HEADERS_GEOMETRY}
    DESTINATION include/${PROJECT_NAME}/Geometry)
install(FILES ${PUBLIC_HEADERS_UTILITIES}
    DESTINATION include/${PROJECT_NAME}/Utilities)
