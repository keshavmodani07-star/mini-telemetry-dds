#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string>

// Core
#include "core/telemetry.hpp"
#include "core/channel.hpp"
#include "core/telemetry_serializer.hpp"
// DDS
#include "dds_io/dds_participant.hpp"
#include "dds_io/dds_publisher.hpp"

static std::atomic<bool> running{true};

void signal_handler(int)
{
    running.store(false);
}

// ---------------- Time helper ----------------
std::uint64_t now_ns()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

// ---------------- Sensor Thread ----------------
void sensor_thread(
    std::uint32_t sensor_id,
    const std::string& unit,
    double base_value,
    core::Channel<core::TelemetryMessage>& channel,
    std::atomic<bool>& running
)
{
    std::uint64_t seq = 0;

    while (running.load())
    {
        core::TelemetryMessage msg{};
        msg.sensor_id    = sensor_id;
        msg.value        = base_value + (std::rand() % 100) * 0.01;
        msg.unit         = unit;
        msg.timestamp_ns = now_ns();
        msg.sequence     = seq++;

        if (!channel.push(msg))
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "[sensor " << sensor_id << "] exiting\n";
}

// ---------------- Publisher Thread ----------------
void publisher_thread(
    core::Channel<core::TelemetryMessage>& channel,
    dds_io::DDSPublisher& publisher
)
{
    core::TelemetryMessage msg;

    while (channel.pop(msg))
    {
auto msg = core::TelemetrySerializer::serialize(msg)
        // 🔹 LOG what we publish
        std::cout << "[sensor_hub] publish: "
                  << "id=" << msg.sensor_id
                  << " value=" << msg.value
                  << " unit=" << msg.unit
                  << " seq=" << msg.sequence
                  << std::endl;

        publisher.publish(payload);
    }

    std::cout << "[publisher] exiting\n";
}

// ---------------- main ----------------
int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "[sensor_hub] Starting up...\n";

    using TelemetryChannel = core::Channel<core::TelemetryMessage>;
    TelemetryChannel channel;

    // DDS setup
    dds_io::DDSParticipant participant;
    dds_io::DDSPublisher publisher(participant.participant());

    // Sensor threads
    std::thread temp_sensor(
        sensor_thread, 1, "C", 25.0,
        std::ref(channel), std::ref(running)
    );

    std::thread pressure_sensor(
        sensor_thread, 2, "kPa", 101.0,
        std::ref(channel), std::ref(running)
    );

    std::thread flow_sensor(
        sensor_thread, 3, "L/min", 10.0,
        std::ref(channel), std::ref(running)
    );

    // Publisher thread
    std::thread publisher_worker(
        publisher_thread,
        std::ref(channel),
        std::ref(publisher)
    );

    // Wait for shutdown
    while (running.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[sensor_hub] Shutting down...\n";

    channel.shutdown();

    temp_sensor.join();
    pressure_sensor.join();
    flow_sensor.join();
    publisher_worker.join();

    std::cout << "[sensor_hub] Shutdown complete.\n";
    return 0;
}
