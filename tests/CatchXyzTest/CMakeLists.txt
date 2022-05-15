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
    GIT_TAG "v2.11.1"
    )
FetchContent_MakeAvailable(catch)

add_executable(CatchXyzTest
    CatchXyzTestMain.cpp
    test_Approx.cpp
    )

target_link_libraries(CatchXyzTest
    Xyz::Xyz
    Catch2::Catch2
    )
