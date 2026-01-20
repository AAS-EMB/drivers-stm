#include <gtest/gtest.h>
#include <steady_clock.hpp>

using namespace std::chrono_literals;
using namespace driver;

template<Clock Cl>
class stopwatch {
public:
    using duration = typename Cl::duration;

    void start() noexcept {
        m_start = Cl::now();
    }

    bool expired(duration timeout) const noexcept {
        return (Cl::now() - m_start) >= timeout;
    }

    duration elapsed() const noexcept {
        return Cl::now() - m_start;
    }

private:
    typename Cl::time_point m_start{};
};

TEST(SteadyClock, StartsAtZero) {
    steady_clock::traits::reset();
    EXPECT_EQ(steady_clock::now().time_since_epoch().count(), 0);
}

TEST(SteadyClock, AdvancesCorrectly) {
    steady_clock::traits::reset();

    steady_clock::traits::advance(steady_clock::duration{10ns}.count());
    EXPECT_EQ(steady_clock::now().time_since_epoch().count(), 10);

    steady_clock::traits::advance(steady_clock::duration{5ns}.count());
    EXPECT_EQ(steady_clock::now().time_since_epoch().count(), 15);
}

TEST(SteadyClock, IsMonotonic) {
    steady_clock::traits::reset();

    auto t1 = steady_clock::now();
    steady_clock::traits::advance(steady_clock::duration{1ns}.count());
    auto t2 = steady_clock::now();

    EXPECT_GT(t2, t1);
}

TEST(Stopwatch, NotExpiredInitially) {
    steady_clock::traits::reset();

    stopwatch<steady_clock> timer;
    timer.start();

    EXPECT_FALSE(timer.expired(10ns));
}

TEST(Stopwatch, ExpiresAfterTimeout) {
    steady_clock::traits::reset();

    stopwatch<steady_clock> timer;
    timer.start();

    steady_clock::traits::advance(steady_clock::duration{10ns}.count());

    EXPECT_TRUE(timer.expired(10ns));
}

TEST(Stopwatch, ElapsedTimeCorrect) {
    steady_clock::traits::reset();

    stopwatch<steady_clock> timer;
    timer.start();

    steady_clock::traits::advance(steady_clock::duration{42ns}.count());

    EXPECT_EQ(timer.elapsed(), 42ns);
}

TEST(Stopwatch, LargeJumpStillWorks) {
    steady_clock::traits::reset();

    stopwatch<steady_clock> timer;
    timer.start();

    steady_clock::traits::advance(steady_clock::duration{1s}.count());

    EXPECT_TRUE(timer.expired(500ms));
}
