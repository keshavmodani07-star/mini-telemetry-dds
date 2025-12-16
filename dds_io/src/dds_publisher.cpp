#include "dds_io/dds_publisher.hpp"
#include "telemetry.h"

#include <stdexcept>
#include <cstring>

namespace dds_io
{

DDSPublisher::DDSPublisher(dds_entity_t participant)
{
    topic_ = dds_create_topic(
        participant,
        &telemetry_TelemetryPacket_desc,
        "lab/telemetry",
        nullptr,
        nullptr);

    if (topic_ < 0)
        throw std::runtime_error("Failed to create DDS topic");

    writer_ = dds_create_writer(participant, topic_, nullptr, nullptr);
    if (writer_ < 0)
        throw std::runtime_error("Failed to create DDS writer");
}

DDSPublisher::~DDSPublisher()
{
    if (writer_ != DDS_ENTITY_NIL)
        dds_delete(writer_);
    if (topic_ != DDS_ENTITY_NIL)
        dds_delete(topic_);
}

void DDSPublisher::publish(const std::vector<std::uint8_t>& payload)
{
    telemetry_TelemetryPacket msg{};
    msg.payload._length = payload.size();
    msg.payload._buffer = const_cast<uint8_t*>(payload.data());

    dds_write(writer_, &msg);
}

} // namespace dds_io

