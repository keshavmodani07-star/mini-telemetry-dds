#pragma once

#include <dds/dds.h>
#include <vector>
#include <cstdint>

namespace dds_io
{

class DDSPublisher
{
public:
    DDSPublisher(dds_entity_t participant);
    ~DDSPublisher();

    void publish(const std::vector<std::uint8_t>& payload);

private:
    dds_entity_t topic_{DDS_ENTITY_NIL};
    dds_entity_t writer_{DDS_ENTITY_NIL};
};

} // namespace dds_io

