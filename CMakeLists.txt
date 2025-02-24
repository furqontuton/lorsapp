# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.10)
project(CMakeProject1)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Users/FATTA IT/source/repos/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Add executable
add_executable(CMakeProject1 main.cpp "handlers.h" "handlers.cpp" "getdb.h" "getdb.cpp")

# Manually include httplib
target_include_directories(CMakeProject1 PRIVATE "C:/Users/FATTA IT/source/repos/vcpkg/installed/x64-windows/include")

find_package(ODBC REQUIRED)

target_link_libraries(CMakeProject1 PRIVATE ODBC::ODBC)