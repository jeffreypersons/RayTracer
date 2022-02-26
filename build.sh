#!/bin/bash

BUILD_TYPE=${1:-Release}
mkdir $BUILD_TYPE >/dev/null
pushd $BUILD_TYPE
cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} && cmake --build . && cmake --install . --prefix .
popd
