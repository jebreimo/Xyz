#!/bin/bash

XYZ_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ ! -d release.Xyz ]
then
    mkdir release.Xyz
fi

cd release.Xyz
cmake -DCMAKE_BUILD_TYPE=Release $XYZ_DIR
make install
cd ..

if [ ! -d debug.Xyz ]
then
    mkdir debug.Xyz
fi

cd debug.Xyz
cmake -DCMAKE_BUILD_TYPE=Debug $XYZ_DIR
make install
cd ..
