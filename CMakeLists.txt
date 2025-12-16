cmake_minimum_required(VERSION 3.16)

project(mini_telemetry_dds
    VERSION 0.1.0
    DESCRIPTION "Mini telemetry system over DDS"
    LANGUAGES C CXX
)

enable_testing()


# ------------------------------------------------------------------------------
# Global build settings
# ------------------------------------------------------------------------------

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Export compile commands (VS Code / clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# Compiler warnings
# ------------------------------------------------------------------------------

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ------------------------------------------------------------------------------
# Subdirectories
# ------------------------------------------------------------------------------

add_subdirectory(core)
add_subdirectory(dds_io)
add_subdirectory(apps)
add_subdirectory(tests)
