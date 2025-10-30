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

## Quick Start Guide (Windows)

**The simplest way to build and run:**

1. Install **Visual Studio Community** (free): https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++" during installation

2. Install **vcpkg** and **GLFW**:
   ```cmd
   cd C:\
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg install glfw3:x64-windows
   .\vcpkg integrate install
   ```

3. Open `Pacman.sln` in Visual Studio

4. Press `Ctrl+Shift+B` to build, then `F5` to run!

**That's it!** No CMake needed for this method.

---

## Building on Windows

### Prerequisites

#### 1. Visual Studio (2017 or later) - **REQUIRED**
   - Download from https://visualstudio.microsoft.com/downloads/
   - **Community Edition is free** and works perfectly
   - During installation, select the **"Desktop development with C++"** workload
   - This is required for compiling C++ code

#### 2. CMake (version 3.15 or higher) - **Optional** (only needed for Method 2)

   **Note:** If you're using Method 1 (Visual Studio solution), you DON'T need CMake!

   Choose one of these installation methods:

   **Option A: Using Visual Studio Installer (Easiest)**
   1. Open Visual Studio Installer
   2. Click "Modify" on your Visual Studio installation
   3. Go to "Individual components" tab
   4. Search for "CMake"
   5. Check "CMake tools for Windows"
   6. Click "Modify" to install

   **Option B: Download Installer from CMake.org**
   1. Go to https://cmake.org/download/
   2. Download the Windows x64 Installer: `cmake-3.xx.x-windows-x86_64.msi`
   3. Run the installer
   4. **Important:** During installation, select "Add CMake to the system PATH for all users"
   5. Complete the installation
   6. Verify by opening a new Command Prompt and typing: `cmake --version`

   **Option C: Using Chocolatey Package Manager**

   If you have Chocolatey installed:
   ```cmd
   choco install cmake
   ```

   To install Chocolatey first (if needed):
   1. Open PowerShell as Administrator
   2. Run:
      ```powershell
      Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
      ```
   3. Then install CMake: `choco install cmake`

   **Option D: Using winget (Windows 11 or Windows 10 with App Installer)**
   ```cmd
   winget install Kitware.CMake
   ```

#### 3. GLFW Library - **REQUIRED**

   **Option 1: Using vcpkg (Recommended - Automatic)**

   1. Open Command Prompt or PowerShell
   2. Install vcpkg:
      ```cmd
      cd C:\
      git clone https://github.com/Microsoft/vcpkg.git
      cd vcpkg
      .\bootstrap-vcpkg.bat
      ```
   3. Install GLFW:
      ```cmd
      .\vcpkg install glfw3:x64-windows
      ```
   4. Integrate with Visual Studio:
      ```cmd
      .\vcpkg integrate install
      ```
   5. Set environment variable (optional but helpful):
      - Right-click "This PC" → Properties → Advanced System Settings → Environment Variables
      - Add new System Variable: `VCPKG_ROOT` = `C:\vcpkg`

   **Option 2: Manual installation (Advanced)**
   - Download pre-compiled binaries from https://www.glfw.org/download.html
   - Extract and note the installation path
   - You'll need to manually configure include/library paths

### Build Instructions

#### Method 1: Using Visual Studio or Rider (Easiest)

The repository includes a pre-configured Visual Studio solution file.

1. **Install GLFW via vcpkg:**
   ```cmd
   vcpkg install glfw3:x64-windows
   vcpkg integrate install
   ```

2. **Open the solution:**
   - Double-click `Pacman.sln` to open in Visual Studio
   - Or open `Pacman.sln` in JetBrains Rider

3. **Set build configuration:**
   - Select `Release` or `Debug` configuration
   - Select `x64` platform

4. **Build:**
   - Press `Ctrl+Shift+B` or click Build → Build Solution
   - Or in Rider: Build → Build Solution

5. **Run:**
   - Press `F5` to run with debugging
   - Or `Ctrl+F5` to run without debugging
   - Executable will be in `bin/Release/Pacman.exe` or `bin/Debug/Pacman.exe`

**Note:** The solution uses `Directory.Build.props` to automatically find vcpkg. Make sure you either:
- Set the `VCPKG_ROOT` environment variable to your vcpkg installation path, OR
- Have vcpkg installed at `C:\vcpkg\` or `%USERPROFILE%\vcpkg\`

#### Method 2: Using CMake with vcpkg

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
