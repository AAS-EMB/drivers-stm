#include <gtest/gtest.h>
#include <system_clock.hpp>

using namespace std::chrono_literals;
using namespace driver;

TEST(SystemClock, StartsFromEpoch) {
    system_clock::traits::reset();

    auto t = system_clock::now();
    EXPECT_EQ(t.time_since_epoch(), 0ms);
}

TEST(SystemClock, AdvancesCorrectly) {
    system_clock::traits::reset();
    system_clock::traits::advance(system_clock::duration{1500ms}.count());

    auto t = system_clock::now();
    EXPECT_EQ(t.time_since_epoch(), 1500ms);
}

TEST(SystemClock, TimeJumpForward) {
    system_clock::traits::reset();
    system_clock::traits::advance(system_clock::duration{10s}.count());

    auto t1 = system_clock::now();

    system_clock::traits::advance(system_clock::duration{5min}.count()); // NTP / GNSS sync

    auto t2 = system_clock::now();
    EXPECT_EQ(t2 - t1, 5min);
}