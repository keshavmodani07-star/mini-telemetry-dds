#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>

static std::atomic<bool> running{true};

void signal_handler(int)
{
    running.store(false);
}

int main()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::cout << "[sensor_hub] Starting up..." << std::endl;

    while (running.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "[sensor_hub] Shutting down cleanly." << std::endl;
    return 0;
}
