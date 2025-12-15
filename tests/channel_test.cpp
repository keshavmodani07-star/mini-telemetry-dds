#include <core/channel.hpp>

#include <gtest/gtest.h>
#include <thread>
#include <vector>

using core::Channel;

TEST(ChannelTest, SingleProducerSingleConsumer)
{
    Channel<int> channel;

    std::thread producer([&]() {
        for (int i = 0; i < 10; ++i)
        {
            EXPECT_TRUE(channel.push(i));
        }
        channel.shutdown();
    });

    std::thread consumer([&]() {
        int value = 0;
        int count = 0;
        while (channel.pop(value))
        {
            EXPECT_EQ(value, count);
            ++count;
        }
        EXPECT_EQ(count, 10);
    });

    producer.join();
    consumer.join();

    EXPECT_EQ(channel.pushed_count(), 10);
    EXPECT_EQ(channel.popped_count(), 10);
}

TEST(ChannelTest, ShutdownWakesConsumer)
{
    Channel<int> channel;

    std::thread consumer([&]() {
        int value = 0;
        bool result = channel.pop(value);
        EXPECT_FALSE(result);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    channel.shutdown();

    consumer.join();
}

