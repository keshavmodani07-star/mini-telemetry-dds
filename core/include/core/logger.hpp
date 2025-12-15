#pragma once

#include <string>

namespace core
{

enum class LogLevel
{
    Info,
    Warn,
    Error
};

void log(LogLevel level, const std::string& message);

} // namespace core

