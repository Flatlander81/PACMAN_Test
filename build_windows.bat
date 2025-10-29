@echo off
echo ========================================
echo   Pac-Man Game - Windows Build Script
echo ========================================
echo.

REM Check if vcpkg path is set
if "%VCPKG_ROOT%"=="" (
    echo WARNING: VCPKG_ROOT environment variable not set.
    echo If you have vcpkg installed, set it with:
    echo   set VCPKG_ROOT=C:\path\to\vcpkg
    echo.
    echo Attempting build without vcpkg...
    echo.
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring project with CMake...
if "%VCPKG_ROOT%"=="" (
    cmake .. -G "Visual Studio 16 2019" -A x64
) else (
    cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
)

if %errorlevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Please ensure:
    echo   1. CMake is installed and in PATH
    echo   2. Visual Studio 2019 is installed
    echo   3. GLFW is installed via vcpkg or system-wide
    echo.
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo.
    echo ERROR: Build failed!
    echo Check the error messages above.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build Successful!
echo ========================================
echo.
echo Executable location: build\bin\Release\Pacman.exe
echo Or copied to root: Pacman.exe
echo.
echo Run the game by executing Pacman.exe
echo.
pause
