#!/bin/bash

echo "========================================"
echo "  Pac-Man Game - Linux Build Script"
echo "========================================"
echo ""

# Check for required dependencies
echo "Checking dependencies..."

if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake is not installed!"
    echo "Install with: sudo apt-get install cmake"
    exit 1
fi

if ! command -v g++ &> /dev/null; then
    echo "ERROR: g++ is not installed!"
    echo "Install with: sudo apt-get install g++"
    exit 1
fi

# Check for GLFW
if ! pkg-config --exists glfw3; then
    echo "WARNING: GLFW3 not found via pkg-config"
    echo "Install with: sudo apt-get install libglfw3-dev"
    echo "Attempting to continue..."
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo ""
echo "Configuring project with CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake configuration failed!"
    echo "Please ensure all dependencies are installed:"
    echo "  sudo apt-get install cmake g++ libglfw3-dev libgl1-mesa-dev"
    exit 1
fi

# Build
echo ""
echo "Building project..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed!"
    echo "Check the error messages above."
    exit 1
fi

echo ""
echo "========================================"
echo "  Build Successful!"
echo "========================================"
echo ""
echo "Executable location: build/Pacman"
echo "Or copied to root: Pacman"
echo ""
echo "Run the game with: ./Pacman"
echo ""
