#include <gtest/gtest.h>
#include "timer.hpp"

using namespace std::chrono_literals;

class fake_steady_clock {
public:
    using duration   = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<fake_steady_clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept
    { return time_point(duration(s_now_ns)); }

    static void advance(duration d) noexcept
    { s_now_ns += d.count(); }

    static void reset() noexcept
    { s_now_ns = 0; }

private:
    static inline uint64_t s_now_ns = 0;
};

class overflow_extender {
public:
    uint64_t feed(uint32_t value) {
        if (value < last_) {
            ++high_;
        }
        last_ = value;
        return (static_cast<uint64_t>(high_) << 32) | value;
    }

private:
    uint32_t last_ = 0;
    uint32_t high_ = 0;
};

using Timer = stm32_drivers::basic_timer<fake_steady_clock>;

TEST(FakeClock, StartsAtZero) {
    fake_steady_clock::reset();
    EXPECT_EQ(fake_steady_clock::now().time_since_epoch().count(), 0);
}

TEST(FakeClock, AdvancesCorrectly) {
    fake_steady_clock::reset();

    fake_steady_clock::advance(10ns);
    EXPECT_EQ(fake_steady_clock::now().time_since_epoch().count(), 10);

    fake_steady_clock::advance(5ns);
    EXPECT_EQ(fake_steady_clock::now().time_since_epoch().count(), 15);
}

TEST(FakeClock, IsMonotonic) {
    fake_steady_clock::reset();

    auto t1 = fake_steady_clock::now();
    fake_steady_clock::advance(1ns);
    auto t2 = fake_steady_clock::now();

    EXPECT_GT(t2, t1);
}

TEST(BasicTimer, NotExpiredInitially) {
    fake_steady_clock::reset();

    Timer timer;
    timer.start();

    EXPECT_FALSE(timer.expired(10ns));
}

TEST(BasicTimer, ExpiresAfterTimeout) {
    fake_steady_clock::reset();

    Timer timer;
    timer.start();

    fake_steady_clock::advance(10ns);

    EXPECT_TRUE(timer.expired(10ns));
}

TEST(BasicTimer, ElapsedTimeCorrect) {
    fake_steady_clock::reset();

    Timer timer;
    timer.start();

    fake_steady_clock::advance(42ns);

    EXPECT_EQ(timer.elapsed(), 42ns);
}

TEST(BasicTimer, LargeJumpStillWorks) {
    fake_steady_clock::reset();

    Timer timer;
    timer.start();

    fake_steady_clock::advance(1s);

    EXPECT_TRUE(timer.expired(500ms));
}

TEST(OverflowExtender, NoOverflow) {
    overflow_extender ext;

    EXPECT_EQ(ext.feed(1), 1);
    EXPECT_EQ(ext.feed(100), 100);
    EXPECT_EQ(ext.feed(1000), 1000);
}

TEST(OverflowExtender, DetectsWrapAround) {
    overflow_extender ext;

    ext.feed(0xFFFF'FFFE);
    ext.feed(0xFFFF'FFFF);

    auto v = ext.feed(5);

    EXPECT_EQ(v, (1ULL << 32) | 5);
}

TEST(OverflowExtender, MultipleWraps) {
    overflow_extender ext;

    ext.feed(0xFFFF'FFFF);
    ext.feed(1);

    ext.feed(0xFFFF'FFFF);
    auto v = ext.feed(2);

    EXPECT_EQ(v, (2ULL << 32) | 2);
}