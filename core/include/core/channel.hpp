#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>

namespace core
{

template <typename T>
class Channel
{
public:
    Channel()
        : shutdown_(false),
          pushed_count_(0),
          popped_count_(0)
    {}

    // Disable copy
    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;

    // Push an item into the channel
    // Returns false if channel is shutting down
    bool push(const T& item)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (shutdown_)
            {
                return false;
            }
            queue_.push(item);
            ++pushed_count_;
        }
        cv_.notify_one();
        return true;
    }

    // Pop an item from the channel (blocking)
    // Returns false if channel is shut down and empty
    bool pop(T& out)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() {
            return shutdown_ || !queue_.empty();
        });

        if (queue_.empty())
        {
            // shutdown_ must be true here
            return false;
        }

        out = std::move(queue_.front());
        queue_.pop();
        ++popped_count_;
        return true;
    }

    // Signal shutdown and wake all waiting threads
    void shutdown()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            shutdown_ = true;
        }
        cv_.notify_all();
    }

    // Statistics
    std::uint64_t pushed_count() const
    {
        return pushed_count_.load();
    }

    std::uint64_t popped_count() const
    {
        return popped_count_.load();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<T> queue_;

    std::atomic<bool> shutdown_;
    std::atomic<std::uint64_t> pushed_count_;
    std::atomic<std::uint64_t> popped_count_;
};

} // namespace core

