#include <gtest/gtest.h>
#include <coro/coro_timer.hpp>
#include <coro/coro_task.hpp>

using namespace driver;
using namespace driver::async;
using namespace std::chrono_literals;

TEST(CoroTimer, DoesNotFireTooEarly) {
    coro_timer timer{ 100ns };
    steady_clock::traits::reset();
    bool fired = false;

    auto coro = [&]() -> coro_task {
        co_await timer;
        fired = true;
    };

    auto t = coro();

    timer.process();
    EXPECT_FALSE(fired);

    steady_clock::traits::advance(50);
    timer.process();
    EXPECT_FALSE(fired);

    steady_clock::traits::advance(49);
    timer.process();
    EXPECT_FALSE(fired);
}

TEST(CoroTimer, FiresAtDeadline) {
    coro_timer timer{ 100ns };
    steady_clock::traits::reset();
    bool fired = false;

    auto coro = [&]() -> coro_task {
        co_await timer;
        fired = true;
    };

    auto t = coro();

    timer.process();
    EXPECT_FALSE(fired);

    steady_clock::traits::advance(100);
    timer.process();
    EXPECT_TRUE(fired);
}

TEST(CoroTimer, MultipleTimersWork) {
    coro_timer timer1{ 50ns };
    coro_timer timer2{ 100ns };
    steady_clock::traits::reset();
    bool a = false;
    bool b = false;

    auto coro1 = [&]() -> coro_task {
        co_await timer1;
        a = true;
    };

    auto coro2 = [&]() -> coro_task {
        co_await timer2;
        b = true;
    };

    auto t1 = coro1();
    auto t2 = coro2();

    steady_clock::traits::advance(60);
    timer1.process();
    timer2.process();
    EXPECT_TRUE(a);
    EXPECT_FALSE(b);

    steady_clock::traits::advance(40);
    timer1.process();
    timer2.process();
    EXPECT_TRUE(b);
}

TEST(CoroTimer, ReuseTimerSlot) {
    coro_timer timer{ 50ns };

    steady_clock::traits::reset();
    int count = 0;

    auto coro = [&]() -> coro_task {
        co_await timer;
        count++;
        co_await timer;
        count++;
    };

    auto t = coro();

    steady_clock::traits::advance(50);
    timer.process();
    EXPECT_EQ(count, 1);

    steady_clock::traits::advance(50);
    timer.process();
    EXPECT_EQ(count, 2);
}

TEST(CoroTimer, NoPollNoFire) {
    coro_timer timer{ 100ns };

    steady_clock::traits::reset();
    bool fired = false;

    auto coro = [&]() -> coro_task {
        co_await timer;
        fired = true;
    };

    auto t = coro();

    steady_clock::traits::advance(200);
    EXPECT_FALSE(fired);
}