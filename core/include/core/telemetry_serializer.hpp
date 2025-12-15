#pragma once

#include <core/telemetry.hpp>

#include <cstdint>
#include <vector>

namespace core
{

// Serialize TelemetryMessage into a byte buffer
std::vector<std::uint8_t> serialize(const TelemetryMessage& msg);

// Deserialize TelemetryMessage from a byte buffer
// Throws std::runtime_error on failure
TelemetryMessage deserialize(const std::vector<std::uint8_t>& buffer);

} // namespace core

