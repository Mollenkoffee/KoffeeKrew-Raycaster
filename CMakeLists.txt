# CMakeList.txt : CMake project for KoffeeKrew-Raycaster, include source and define
# project-specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("KoffeeKrew-Raycaster")

# Add source to this project's executable.
add_executable(KoffeeKrew-Raycaster "KoffeeKrew-Raycaster.cpp" "KoffeeKrew-Raycaster.h")

# Set C++20 standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KoffeeKrew-Raycaster PROPERTY CXX_STANDARD 20)
endif()

# Set path to vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/atmol/source/repos/KoffeeKrew-Raycaster/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find and link OpenGL, GLFW, and GLEW
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(KoffeeKrew-Raycaster PRIVATE OpenGL::GL glfw GLEW::GLEW)