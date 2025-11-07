cmake_minimum_required(VERSION 3.10)
project(first_sfml)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
#include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Find SFML
find_package(SFML  COMPONENTS graphics window system REQUIRED)

# Create the executable
add_executable(SFMLGame ${SOURCES})

# Link SFML libraries
target_link_libraries(SFMLGame sfml-graphics sfml-window sfml-system)
