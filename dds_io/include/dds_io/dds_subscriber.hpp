#pragma once

#include <dds/dds.h>
#include <vector>
#include <cstdint>

namespace dds_io
{

class DDSSubscriber
{
public:
    DDSSubscriber(dds_entity_t participant);
    ~DDSSubscriber();

    bool take(std::vector<std::uint8_t>& out_payload);

private:
    dds_entity_t topic_{DDS_ENTITY_NIL};
    dds_entity_t reader_{DDS_ENTITY_NIL};
};

} // namespace dds_io

