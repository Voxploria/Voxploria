#!/bin/bash

set -e

if [ -d "build" ]; then
    echo "Cleaning existing build directory..."
    rm -rf build
fi

echo "Creating new build directory..."
mkdir build
cd build

echo "Generating build files with CMake..."
cmake ..

echo "Building the project..."
cmake --build .

echo "Build completed successfully!"
