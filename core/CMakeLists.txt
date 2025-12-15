# Core library CMake configuration

add_library(telemetry_core
    src/logger.cpp
)

target_include_directories(telemetry_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(telemetry_core
    PUBLIC cxx_std_17
)

