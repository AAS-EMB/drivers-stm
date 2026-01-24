#include <gtest/gtest.h>
#include <coro/async_flag.hpp>
#include <coro/coro_task.hpp>

using namespace driver::async;

TEST(AsyncFlag, CoroutineWaitsUntilSet) {
    async_flag flag;
    bool resumed = false;

    auto coro = [&]() -> coro_task {
        co_await flag;
        resumed = true;
    };

    auto t = coro();

    EXPECT_FALSE(resumed);

    flag = true;

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, ImmediateResumeIfAlreadySet) {
    async_flag flag;

    flag = true;

    bool resumed = false;

    auto coro = [&]() -> coro_task {
        co_await flag;
        resumed = true;
    };

    auto t = coro();

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, ClearResetsFlag) {
    async_flag flag;

    flag = true;
    flag = false;

    EXPECT_FALSE(flag.is_set());

    bool resumed = false;

    auto coro = [&]() -> coro_task {
        co_await flag;
        resumed = true;
    };

    auto t = coro();

    EXPECT_FALSE(resumed);
}

TEST(AsyncFlag, MultipleSetIsSafe) {
    async_flag flag;
    bool resumed = false;

    auto coro = [&]() -> coro_task {
        co_await flag;
        resumed = true;
    };

    auto t = coro();

    flag = true;
    flag = true;
    flag = true;

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, DoubleSetDoesNotResumeTwice) {
    async_flag flag;
    int counter = 0;

    auto coro = [&]() -> coro_task {
        co_await flag;
        ++counter;
    };

    auto t = coro();

    flag = true;
    EXPECT_EQ(counter, 1);

    flag = true;
    EXPECT_EQ(counter, 1);
}

TEST(AsyncFlag, ResetAllowsReuse) {
    async_flag flag;
    int counter = 0;

    auto coro1 = [&]() -> coro_task {
        co_await flag;
        ++counter;
    };

    auto t1 = coro1();

    flag = true;
    EXPECT_EQ(counter, 1);

    flag = false;

    auto coro2 = [&]() -> coro_task {
        co_await flag;
        ++counter;
    };

    auto t2 = coro2();

    EXPECT_EQ(counter, 1);

    flag = true;
    EXPECT_EQ(counter, 2);
}

TEST(AsyncFlag, OnlyOneWaiterAllowed) {
    async_flag flag;
    bool a = false;
    bool b = false;

    auto coro = [&]() -> coro_task {
        co_await flag;
        a = true;
    };

    auto t = coro();

#ifndef NDEBUG
    EXPECT_DEATH({
        auto coro = [&]() -> coro_task {
            co_await flag;
            b = true;
        };
        auto t = coro();
    }, "Only one waiter");
#endif
}

TEST(AsyncFlag, NoResumeAfterCompletion) {
    async_flag flag;
    int counter = 0;

    auto coro = [&]() -> coro_task {
        co_await flag;
        ++counter;
    };

    auto t = coro();

    flag = true;
    EXPECT_EQ(counter, 1);

    flag = true;
    EXPECT_EQ(counter, 1);
}

TEST(AsyncFlag, CoroutineOutlivesFlag) {
    coro_task task = []() -> coro_task {
        async_flag flag;
        co_await flag;
    }();

    SUCCEED();
}