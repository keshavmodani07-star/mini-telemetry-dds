#include "core/telemetry_serializer.hpp"

#include <cstring>
#include <stdexcept>

namespace core
{

std::vector<std::uint8_t>
TelemetrySerializer::serialize(const TelemetryMessage& msg)
{
    std::vector<std::uint8_t> buffer;

    auto append = [&](const void* data, size_t size)
    {
        const std::uint8_t* p =
            static_cast<const std::uint8_t*>(data);
        buffer.insert(buffer.end(), p, p + size);
    };

    append(&msg.sensor_id, sizeof(msg.sensor_id));
    append(&msg.value, sizeof(msg.value));
    append(&msg.timestamp_ns, sizeof(msg.timestamp_ns));
    append(&msg.sequence, sizeof(msg.sequence));

    std::uint32_t unit_len =
        static_cast<std::uint32_t>(msg.unit.size());
    append(&unit_len, sizeof(unit_len));
    append(msg.unit.data(), unit_len);

    return buffer;
}

TelemetryMessage
TelemetrySerializer::deserialize(const std::vector<std::uint8_t>& buffer)
{
    TelemetryMessage msg{};
    size_t offset = 0;

    auto read = [&](void* dst, size_t size)
    {
        if (offset + size > buffer.size())
            throw std::runtime_error("Telemetry deserialize overflow");

        std::memcpy(dst, buffer.data() + offset, size);
        offset += size;
    };

    read(&msg.sensor_id, sizeof(msg.sensor_id));
    read(&msg.value, sizeof(msg.value));
    read(&msg.timestamp_ns, sizeof(msg.timestamp_ns));
    read(&msg.sequence, sizeof(msg.sequence));

    std::uint32_t unit_len{};
    read(&unit_len, sizeof(unit_len));

    if (offset + unit_len > buffer.size())
        throw std::runtime_error("Invalid unit length");

    msg.unit.assign(
        reinterpret_cast<const char*>(buffer.data() + offset),
        unit_len
    );

    return msg;
}

} // namespace core

