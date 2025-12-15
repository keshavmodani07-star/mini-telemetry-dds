#include <core/telemetry_serializer.hpp>

#include "telemetry.pb.h"

#include <stdexcept>

namespace core
{

std::vector<std::uint8_t> serialize(const TelemetryMessage& msg)
{
    telemetry::TelemetryMessage proto;

    proto.set_sensor_id(msg.sensor_id);
    proto.set_value(msg.value);
    proto.set_unit(msg.unit);
    proto.set_timestamp_ns(msg.timestamp_ns);
    proto.set_sequence(msg.sequence);

    std::vector<std::uint8_t> buffer(proto.ByteSizeLong());
    if (!proto.SerializeToArray(buffer.data(), buffer.size()))
    {
        throw std::runtime_error("Failed to serialize TelemetryMessage");
    }

    return buffer;
}

TelemetryMessage deserialize(const std::vector<std::uint8_t>& buffer)
{
    telemetry::TelemetryMessage proto;

    if (!proto.ParseFromArray(buffer.data(), buffer.size()))
    {
        throw std::runtime_error("Failed to deserialize TelemetryMessage");
    }

    TelemetryMessage msg;
    msg.sensor_id    = proto.sensor_id();
    msg.value        = proto.value();
    msg.unit         = proto.unit();
    msg.timestamp_ns = proto.timestamp_ns();
    msg.sequence     = proto.sequence();

    return msg;
}

} // namespace core

