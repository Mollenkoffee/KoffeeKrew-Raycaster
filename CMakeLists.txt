cmake_minimum_required(VERSION 3.8)

# Project name
project("KoffeeKrew-Raycaster")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/KoffeeKrew-Raycaster.cpp
    src/Map.cpp
    src/Player.cpp
    src/Renderer.cpp
    src/Raycaster.cpp
    src/Texture.cpp
)

# Header files
set(HEADERS
    include/KoffeeKrew-Raycaster.h
    include/Map.h
    include/Player.h
    include/Renderer.h
    include/Raycaster.h
    include/Constants.h
    include/Texture.h
)

# Add the executable
add_executable(KoffeeKrew-Raycaster ${SOURCES} ${HEADERS})

# Find and link OpenGL, GLFW, and GLEW
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Link libraries
target_link_libraries(KoffeeKrew-Raycaster PRIVATE OpenGL::GL glfw GLEW::GLEW)