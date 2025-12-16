#include "dds_io/dds_participant.hpp"

#include <stdexcept>

namespace dds_io
{

DDSParticipant::DDSParticipant(std::uint32_t domain_id)
{
    participant_ = dds_create_participant(domain_id, nullptr, nullptr);
    if (participant_ < 0)
    {
        throw std::runtime_error("Failed to create DDS participant");
    }
}

DDSParticipant::~DDSParticipant()
{
    if (participant_ != DDS_ENTITY_NIL)
    {
        dds_delete(participant_);
    }
}

dds_entity_t DDSParticipant::participant() const
{
    return participant_;
}

} // namespace dds_io

