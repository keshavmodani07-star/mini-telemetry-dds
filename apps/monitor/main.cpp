#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <vector>

// Core
#include "core/telemetry.hpp"
#include "core/telemetry_serializer.hpp"

// DDS
#include "dds_io/dds_participant.hpp"
#include "dds_io/dds_subscriber.hpp"

static std::atomic<bool> running{true};

void signal_handler(int)
{
    running.store(false);
}

int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "[monitor] Starting DDS monitor...\n";

    dds_io::DDSParticipant participant;
    dds_io::DDSSubscriber subscriber(participant.participant());

    std::vector<std::uint8_t> payload;

    while (running.load())
    {
        if (subscriber.take(payload))
        {
            try
            {
                auto msg = core::TelemetrySerializer::deserialize(payload);

                std::cout << "[monitor] recv: "
                          << "id=" << msg.sensor_id
                          << " value=" << msg.value
                          << " unit=" << msg.unit
                          << " seq=" << msg.sequence
                          << "\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << "[monitor] deserialize error: "
                          << e.what() << "\n";
            }
        }
        else
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(50));
        }
    }

    std::cout << "[monitor] Shutting down cleanly.\n";
    return 0;
}

