#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <vector>

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

    // DDS setup
    dds_io::DDSParticipant participant;
    dds_io::DDSSubscriber subscriber(participant.participant());

    std::vector<std::uint8_t> payload;

    while (running.load())
    {
        if (subscriber.take(payload))
        {
            std::cout << "[monitor] Received packet ("
                      << payload.size() << " bytes)\n";
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::cout << "[monitor] Shutting down cleanly.\n";
    return 0;
}

