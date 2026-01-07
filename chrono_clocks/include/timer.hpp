#pragma once

#include "steady_clock.hpp"

namespace stm32_drivers {

template<typename Clock = steady_clock>
class basic_timer {
public:
    using duration   = typename Clock::duration;
    using time_point = typename Clock::time_point;

    void start() noexcept {
        m_start = Clock::now();
    }

    bool expired(duration timeout) const noexcept {
        return (Clock::now() - m_start) >= timeout;
    }

    duration elapsed() const noexcept {
        return Clock::now() - m_start;
    }

private:
    time_point m_start{};
};

}
