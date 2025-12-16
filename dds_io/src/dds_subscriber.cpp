#include "dds_io/dds_subscriber.hpp"
#include "telemetry.h"

#include <dds/dds.h>
#include <stdexcept>

namespace dds_io
{

DDSSubscriber::DDSSubscriber(dds_entity_t participant)
{
    topic_ = dds_create_topic(
        participant,
        &telemetry_TelemetryPacket_desc,
        "lab/telemetry",
        nullptr,
        nullptr);

    if (topic_ < 0)
        throw std::runtime_error("Failed to create DDS topic");

    reader_ = dds_create_reader(participant, topic_, nullptr, nullptr);
    if (reader_ < 0)
        throw std::runtime_error("Failed to create DDS reader");
}

DDSSubscriber::~DDSSubscriber()
{
    if (reader_ != DDS_ENTITY_NIL)
        dds_delete(reader_);
    if (topic_ != DDS_ENTITY_NIL)
        dds_delete(topic_);
}

bool DDSSubscriber::take(std::vector<std::uint8_t>& out_payload)
{
    void* samples[1] = { nullptr };
    dds_sample_info_t infos[1];

    int ret = dds_take(
        reader_,
        samples,
        infos,
        1,
        DDS_ANY_STATE
    );

    if (ret <= 0)
        return false;

    if (!infos[0].valid_data)
    {
        dds_return_loan(reader_, samples, ret);
        return false;
    }

    auto* msg =
        static_cast<telemetry_TelemetryPacket*>(samples[0]);

    out_payload.assign(
        msg->payload._buffer,
        msg->payload._buffer + msg->payload._length
    );

    dds_return_loan(reader_, samples, ret);
    return true;
}

} // namespace dds_io

