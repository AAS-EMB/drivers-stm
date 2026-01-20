#include <gtest/gtest.h>
#include <coro/async_flag.hpp>

using namespace driver;

struct test_task {
    struct promise_type {
        test_task get_return_object() noexcept { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }
    };
};

TEST(AsyncFlag, InitiallyNotReady) {
    async_flag flag;

    EXPECT_FALSE(flag.await_ready());
}

TEST(AsyncFlag, SetMakesReady) {
    async_flag flag;

    flag = true;

    EXPECT_TRUE(flag.await_ready());
}

TEST(AsyncFlag, CoroutineWaitsUntilSet) {
    async_flag flag;
    bool resumed = false;

    auto coro = [&]() -> test_task {
        co_await flag;
        resumed = true;
    };

    coro();

    EXPECT_FALSE(resumed);

    flag = true;

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, ImmediateResumeIfAlreadySet) {
    async_flag flag;

    flag = true;

    bool resumed = false;

    auto coro = [&]() -> test_task {
        co_await flag;
        resumed = true;
    };

    coro();

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, ClearResetsFlag) {
    async_flag flag;

    flag = true;
    flag = false;

    EXPECT_FALSE(flag.await_ready());

    bool resumed = false;

    auto coro = [&]() -> test_task {
        co_await flag;
        resumed = true;
    };

    coro();

    EXPECT_FALSE(resumed);
}

TEST(AsyncFlag, MultipleSetIsSafe) {
    async_flag flag;
    bool resumed = false;

    auto coro = [&]() -> test_task {
        co_await flag;
        resumed = true;
    };

    coro();

    flag = true;
    flag = true;
    flag = true;

    EXPECT_TRUE(resumed);
}

TEST(AsyncFlag, AwaitResumeReturnsTrue) {
    async_flag flag;
    bool result = false;

    auto coro = [&]() -> test_task {
        result = co_await flag;
    };

    coro();
    flag = true;

    EXPECT_TRUE(result);
}

TEST(AsyncFlag, OnlyOneWaiterSupported) {
    async_flag flag;
    int counter = 0;

    auto coro1 = [&]() -> test_task {
        co_await flag;
        ++counter;
    };

    auto coro2 = [&]() -> test_task {
        co_await flag;
        ++counter;
    };

    coro1();
    coro2();

    flag = true;

    EXPECT_EQ(counter, 1);
}