cmake_minimum_required(VERSION 3.15)
project(Pacman VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 CONFIG)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found via find_package, trying pkg-config")
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLFW REQUIRED glfw3)
    else()
        message(FATAL_ERROR "GLFW not found. Please install GLFW or use vcpkg: vcpkg install glfw3")
    endif()
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Renderer.cpp
    src/Level.cpp
    src/Pacman.cpp
    src/Ghost.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
)

# Link libraries
if(glfw3_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        glfw
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        ${GLFW_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
endif()

# Windows specific settings
if(WIN32)
    # Hide console window for release builds
    if(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE OFF  # Set to ON to hide console
        )
    endif()
endif()

# Copy executable to root directory after build (for easy running)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:${PROJECT_NAME}>
    ${CMAKE_SOURCE_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
)
