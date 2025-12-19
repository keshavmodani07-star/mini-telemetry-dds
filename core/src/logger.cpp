#include <core/logger.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace core
{

namespace
{
std::mutex log_mutex;

const char* to_string(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info:  return "INFO";
        case LogLevel::Warn:  return "WARN";
        case LogLevel::Error: return "ERROR";
        default:              return "UNKNOWN";
    }
}
} // anonymous namespace

void log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    const auto now = std::chrono::system_clock::now();
    const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "] "
              << "[" << to_string(level) << "] "
              << message
              << std::endl;
}

} // namespace core

