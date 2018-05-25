#Effective Modern CMake
#https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1

cmake_minimum_required (VERSION 3.10)

# CMakeLists files in this project can refer to the root source directory
# of the project as ${CGEM_SOURCE_DIR} and
# to the root binary directory of the project as ${CGEM_BINARY_DIR}.
project (CGEM)

# Use C++17
#https://crascit.com/2015/03/28/enabling-cxx11-in-cmake/
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Install inside build directory
set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# CGEM depends on...
#include(${CMAKE_CURRENT_SOURCE_DIR}/external_packages.cmake)

add_subdirectory (src/c)
add_subdirectory (src/cpp)

enable_testing()
find_package(GTest REQUIRED)
add_subdirectory (test)

# at the end show the settings and tools
include (${CMAKE_CURRENT_SOURCE_DIR}/log_variables.cmake)
