#pragma once

#include <cstdint>
#include <string>


namespace core
{

struct TelemetryMessage
{
    std::uint32_t sensor_id{};
    double value{};
    std::string unit;
    std::uint64_t timestamp_ns{};
    std::uint64_t sequence{};
};

} // namespace core

