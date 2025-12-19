#pragma once

#include <vector>
#include <cstdint>
#include "core/telemetry.hpp"

namespace core
{

class TelemetrySerializer
{
public:
    static std::vector<std::uint8_t>
    serialize(const TelemetryMessage& msg);

    static TelemetryMessage
    deserialize(const std::vector<std::uint8_t>& buffer);
};

} // namespace core
