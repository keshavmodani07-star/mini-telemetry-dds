#pragma once

#include <dds/dds.h>
#include <cstdint>

namespace dds_io
{

class DDSParticipant
{
public:
    explicit DDSParticipant(std::uint32_t domain_id = 0);
    ~DDSParticipant();

    dds_entity_t participant() const;

private:
    dds_entity_t participant_{DDS_ENTITY_NIL};
};

} // namespace dds_io

