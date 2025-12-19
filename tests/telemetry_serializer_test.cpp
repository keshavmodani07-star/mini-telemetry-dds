#include <gtest/gtest.h>

#include <core/telemetry.hpp>
#include <core/telemetry_serializer.hpp>

TEST(TelemetrySerializationTest, RoundTripSerialization)
{
    core::TelemetryMessage original{};
    original.sensor_id = 42;
    original.value = 36.5;
    original.unit = "C";
    original.timestamp_ns = 123456789;
    original.sequence = 7;

    // Serialize
    auto buffer = core::serialize(original);

    ASSERT_FALSE(buffer.empty());

    // Deserialize
    auto decoded = core::deserialize(buffer);

    EXPECT_EQ(decoded.sensor_id, original.sensor_id);
    EXPECT_DOUBLE_EQ(decoded.value, original.value);
    EXPECT_EQ(decoded.unit, original.unit);
    EXPECT_EQ(decoded.timestamp_ns, original.timestamp_ns);
    EXPECT_EQ(decoded.sequence, original.sequence);
}

