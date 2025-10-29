# Pac-Man Game

A classic Pac-Man game implementation in C++ using OpenGL and GLFW.

## Features

- Classic Pac-Man gameplay
- 2 unique levels
- 4 ghosts with different AI behaviors (Blinky, Pinky, Inky, Clyde)
- Power pellets that allow you to eat ghosts
- Score tracking and lives system
- WASD keyboard controls
- Smooth animations

## Controls

- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **R** - Restart Game
- **ESC** - Exit Game

## Building on Windows

### Prerequisites

1. **CMake** (version 3.15 or higher)
   - Download from https://cmake.org/download/
   - Or install via Chocolatey: `choco install cmake`

2. **Visual Studio** (2017 or later)
   - Download from https://visualstudio.microsoft.com/
   - Make sure to install the "Desktop development with C++" workload

3. **GLFW Library**
   - Option 1: Using vcpkg (recommended)
     ```cmd
     git clone https://github.com/Microsoft/vcpkg.git
     cd vcpkg
     .\bootstrap-vcpkg.bat
     .\vcpkg install glfw3:x64-windows
     ```
   - Option 2: Manual installation
     - Download pre-compiled binaries from https://www.glfw.org/download.html
     - Extract and note the installation path

### Build Instructions

#### Using vcpkg (Recommended)

```cmd
# From the repository root
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

#### Using Visual Studio directly

```cmd
# From the repository root
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

Or open the generated `Pacman.sln` file in Visual Studio and build from there.

#### Manual GLFW installation

If you installed GLFW manually, provide the path:

```cmd
mkdir build
cd build
cmake .. -DGLFW_DIR=[path to GLFW]
cmake --build . --config Release
```

### Running the Game

After building, the executable will be in:
- `build/bin/Release/Pacman.exe`
- Or copied to the root directory as `Pacman.exe`

Simply double-click the executable or run from command line:

```cmd
.\Pacman.exe
```

## Building on Linux

### Prerequisites

```bash
sudo apt-get update
sudo apt-get install cmake g++ libglfw3-dev libgl1-mesa-dev
```

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Game

```bash
./Pacman
```

## Game Rules

1. Eat all the pellets to complete a level
2. Avoid the ghosts or lose a life
3. Eat power pellets to temporarily turn ghosts blue and vulnerable
4. Eat blue ghosts for bonus points
5. Complete both levels to win!

### Scoring
- Regular pellet: 10 points
- Power pellet: 50 points
- Ghost: 200 points

## Ghost Behaviors

- **Blinky (Red)**: Chases Pac-Man directly
- **Pinky (Pink)**: Tries to ambush Pac-Man by targeting ahead
- **Inky (Cyan)**: Unpredictable, uses complex targeting
- **Clyde (Orange)**: Chases when far, retreats when close

## Troubleshooting

### GLFW not found

If CMake can't find GLFW, try:
1. Install via vcpkg and use the toolchain file
2. Set GLFW_DIR environment variable to your GLFW installation
3. Install system-wide and ensure it's in your PATH

### OpenGL errors

Make sure your graphics drivers are up to date. OpenGL is included with Windows but requires proper drivers.

### Build fails on Windows

- Ensure Visual Studio is properly installed with C++ tools
- Try running CMake from the "x64 Native Tools Command Prompt for VS"
- Check that all prerequisites are installed

## License

This is a educational project demonstrating game development with C++ and OpenGL.

## Credits

Inspired by the classic Pac-Man arcade game by Namco.
