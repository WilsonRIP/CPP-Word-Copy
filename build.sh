#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .

echo "Build completed. Run the application with: ./CPPWord" 