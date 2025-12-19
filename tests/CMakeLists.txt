cmake_minimum_required(VERSION 3.16)

enable_testing()

find_package(GTest REQUIRED)

add_executable(channel_tests
    channel_test.cpp
)

target_link_libraries(channel_tests
    PRIVATE
        telemetry_core
        GTest::GTest
        GTest::Main
)

add_test(NAME channel_tests COMMAND channel_tests)
