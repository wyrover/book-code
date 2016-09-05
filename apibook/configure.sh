#!/bin/sh
#
# Simple script to create a build/ subdirectory with Unix makefiles to
# build the API examples for your platform
#

if [ "`which cmake`" == "" ]; then
    echo "You must install CMake first - 'cmake' must be in your path"
    exit
fi

if [ -d "build" ]; then
    rm -rf build
fi
mkdir build
cd build
cmake -G "Unix Makefiles" ..

echo "Enter 'make' in the build/ subdir to build all examples."
