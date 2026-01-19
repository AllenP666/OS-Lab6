#!/bin/bash
set -e

mkdir -p build
cd build

echo "Configuring..."
cmake ..

echo "Building..."
cmake --build .

echo "Build successful!"