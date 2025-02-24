# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

cmake_minimum_required(VERSION 3.10)
project(CMakeProject1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/FATTA IT/source/repos/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "VCPKG toolchain file")


find_package(httplib CONFIG REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(CURL REQUIRED)

target_link_libraries(CMakeProject1 PRIVATE CURL::libcurl)

target_link_libraries(CMakeProject1 PRIVATE httplib::httplib)

target_link_libraries(CMakeProject1 PRIVATE nlohmann_json::nlohmann_json)

# Add executable
add_executable(CMakeProject1 main.cpp handlers.cpp)
