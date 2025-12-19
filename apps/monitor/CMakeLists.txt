add_executable(monitor
    main.cpp
)

target_link_libraries(monitor
    PRIVATE
        telemetry_core
        telemetry_dds_io
)

target_compile_features(monitor
    PRIVATE cxx_std_17
)

