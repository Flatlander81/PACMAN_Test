@echo off
echo ========================================
echo   Pac-Man Game - Windows Build Script
echo ========================================
echo.

REM Detect Visual Studio version
set VS_GENERATOR=
set VS_NAME=

REM Check for Visual Studio 2022
if exist "C:\Program Files\Microsoft Visual Studio\2022\" (
    set VS_GENERATOR=Visual Studio 17 2022
    set VS_NAME=Visual Studio 2022
    goto :vs_found
)

REM Check for Visual Studio 2019
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\" (
    set VS_GENERATOR=Visual Studio 16 2019
    set VS_NAME=Visual Studio 2019
    goto :vs_found
)

REM Check for Visual Studio 2017
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\" (
    set VS_GENERATOR=Visual Studio 15 2017
    set VS_NAME=Visual Studio 2017
    goto :vs_found
)

REM No Visual Studio found
echo ERROR: No Visual Studio installation found!
echo.
echo Please install Visual Studio with C++ development tools:
echo   https://visualstudio.microsoft.com/downloads/
echo.
echo Make sure to select "Desktop development with C++" during installation.
echo.
pause
exit /b 1

:vs_found
echo Found: %VS_NAME%
echo.

REM Check for vcpkg
set VCPKG_PATH=
if not "%VCPKG_ROOT%"=="" (
    set VCPKG_PATH=%VCPKG_ROOT%
    echo Using vcpkg from VCPKG_ROOT: %VCPKG_ROOT%
) else (
    REM Try to find vcpkg in common locations
    if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set VCPKG_PATH=C:\vcpkg
        echo Found vcpkg at: C:\vcpkg
    ) else if exist "%USERPROFILE%\vcpkg\scripts\buildsystems\vcpkg.cmake" (
        set VCPKG_PATH=%USERPROFILE%\vcpkg
        echo Found vcpkg at: %USERPROFILE%\vcpkg
    ) else (
        echo WARNING: vcpkg not found in common locations.
        echo.
        echo GLFW library is required to build this project.
        echo.
        echo Please install vcpkg and GLFW:
        echo   cd C:\
        echo   git clone https://github.com/Microsoft/vcpkg.git
        echo   cd vcpkg
        echo   .\bootstrap-vcpkg.bat
        echo   .\vcpkg install glfw3:x64-windows
        echo   .\vcpkg integrate install
        echo.
        echo Then run this script again.
        echo.
        pause
        exit /b 1
    )
)
echo.

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring project with CMake...
echo Generator: %VS_GENERATOR%
echo Platform: x64
echo.

if "%VCPKG_PATH%"=="" (
    cmake .. -G "%VS_GENERATOR%" -A x64
) else (
    cmake .. -G "%VS_GENERATOR%" -A x64 -DCMAKE_TOOLCHAIN_FILE=%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake
)

if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo ERROR: CMake configuration failed!
    echo ========================================
    echo.
    echo Possible solutions:
    echo   1. Make sure CMake is installed and in PATH
    echo      Download from: https://cmake.org/download/
    echo.
    echo   2. Install GLFW via vcpkg:
    echo      cd C:\vcpkg
    echo      .\vcpkg install glfw3:x64-windows
    echo      .\vcpkg integrate install
    echo.
    echo   3. Make sure %VS_NAME% has C++ tools installed
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Building project...
echo ========================================
echo.
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo ERROR: Build failed!
    echo ========================================
    echo.
    echo Check the error messages above.
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build Successful!
echo ========================================
echo.
echo Executable location: %CD%\bin\Release\Pacman.exe
echo.
cd ..
if exist "Pacman.exe" (
    echo Also copied to: %CD%\Pacman.exe
)
echo.
echo Run the game by executing: Pacman.exe
echo.
pause
