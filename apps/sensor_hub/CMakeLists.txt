add_executable(sensor_hub
    main.cpp
)

target_link_libraries(sensor_hub
    PRIVATE
        telemetry_core
        telemetry_dds_io
)

target_compile_features(sensor_hub
    PRIVATE cxx_std_17
)
