#include "ThreadSafeQueue.h"
#include "helpers/test_helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE("ThreadSafeQueue_monoThread", "[basic][queue]")
{
    ThreadSafeQueue<int> queue(5);

    SECTION("empty")
    {
        int popCount = 0;
        queue.popAll([&](int value) {
            popCount++;
        });
        REQUIRE(popCount == 0);
    }

    SECTION("nominal")
    {
        REQUIRE(queue.push(1));

        int popCount = 0;
        queue.popAll([&](int value) {
            REQUIRE(value == 1);
            popCount++;
        });
        REQUIRE(popCount == 1);

        REQUIRE(queue.push(2));
        REQUIRE(queue.push(3));
        REQUIRE(queue.push(4));
        popCount = 0;
        queue.popAll([&](int value) {
            popCount++;
        });
        REQUIRE(popCount == 3);

        REQUIRE(queue.push(5));
        popCount = 0;
        queue.popAll([&](int value) {
            REQUIRE(value == 5);
            popCount++;
        });
        REQUIRE(popCount == 1);

        popCount = 0;
        queue.popAll([&](int value) {
            popCount++;
        });
        REQUIRE(popCount == 0);
    }

    SECTION("full")
    {
        REQUIRE(queue.push(1));
        REQUIRE(queue.push(2));
        REQUIRE(queue.push(3));
        REQUIRE(queue.push(4));
        REQUIRE(!queue.push(5));
        REQUIRE(!queue.push(6));

        int popCount = 0;
        queue.popAll([&](int value) {
            popCount++;
        });
        REQUIRE(popCount == 4);
    }
}

TEST_CASE("ThreadSafeQueueTest_multiThread", "[multi-thread][queue]")
{
    ThreadSafeQueue<int> queue(5);
    int popCount = 0;

    auto run = [&](std::chrono::milliseconds pushCadence, std::chrono::milliseconds popCadence) {
        auto producer = [&queue, &pushCadence]() {
            for (int i = 0; i < 10; ++i)
            {
                if (!queue.push(i))
                {
                    --i; // If the queue is full, retry the same value
                }
                std::this_thread::sleep_for(pushCadence); // Simulate some delay
            }
        };

        auto consumer = [&queue, &popCount, &popCadence]() {
            while (popCount < 10)
            {
                queue.popAll([&](int value) {
                    REQUIRE(value == popCount++);
                });
                std::this_thread::sleep_for(popCadence); // Simulate some delay
            }
            return popCount;
        };

        std::thread t1(producer);
        std::thread t2(consumer);

        t1.join();
        t2.join();

        REQUIRE(popCount == 10);
    };

    SECTION("push faster than pop")
    {
        run(std::chrono::milliseconds(6), std::chrono::milliseconds(20));
    }
    SECTION("pop faster than push")
    {
        run(std::chrono::milliseconds(20), std::chrono::milliseconds(6));
    }
    SECTION("race")
    {
        run(std::chrono::milliseconds(1), std::chrono::milliseconds(1));
    }
}
