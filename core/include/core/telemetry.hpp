#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace core
{

struct Telemetry
{
    std::string sensor_id;   // e.g. "temperature"
    double value;            // sensor reading
    std::string unit;        // e.g. "C", "bar"
    std::uint64_t sequence;  // monotonically increasing
    std::chrono::steady_clock::time_point timestamp;
};

} // namespace core

