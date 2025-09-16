# Top-level CMake configuration for data_store_module
cmake_minimum_required(VERSION 3.10)
project(data_store_module)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use clang as the C compiler (optional, comment out if not needed)
set(CMAKE_C_COMPILER "clang")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add src and test directories
add_subdirectory(src)
add_subdirectory(test)
