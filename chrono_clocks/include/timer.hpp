#pragma once

#include "steady_clock.hpp"

namespace driver {

template<Clock Cl = steady_clock>
class basic_timer {
public:
    using duration   = typename Cl::duration;
    using time_point = typename Cl::time_point;

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
    time_point m_start{};
};

}
