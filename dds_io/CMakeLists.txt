cmake_minimum_required(VERSION 3.16)


# DDS IO library (Cyclone DDS C API wrapper)


add_library(telemetry_dds_io
    src/dds_participant.cpp
    src/dds_publisher.cpp
    src/dds_subscriber.cpp
    generated_c/telemetry.c
)


# Include directories

target_include_directories(telemetry_dds_io
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/generated_c
)


# Find Cyclone DDS (C API)

find_package(CycloneDDS REQUIRED)


# Link against Cyclone DDS C library

target_link_libraries(telemetry_dds_io
    PUBLIC
        CycloneDDS::ddsc
)

# ----------------------------------------
# Enforce C++17 for all users of this lib
# ----------------------------------------

target_compile_features(telemetry_dds_io
    PUBLIC cxx_std_17
)
