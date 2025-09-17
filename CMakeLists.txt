cmake_minimum_required(VERSION 3.5)
project(pocket_dynamic_array_tests C)

# Use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add tests subdirectory
enable_testing()
add_subdirectory(tests)
