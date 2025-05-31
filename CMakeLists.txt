# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.15)

# Define the project name and version
project(dataexchange VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include Conan's toolchain and dependencies
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common/include)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(cryptopp REQUIRED)

add_subdirectory(client)
add_subdirectory(server)
