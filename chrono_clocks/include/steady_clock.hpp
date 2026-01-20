#pragma once

#include <chrono>
#include <cstdint>
#include <platform.hpp>
#include <clock_concept.hpp>

namespace driver {

class steady_clock {
public:
    using traits     = chrono_traits;
    using rep        = typename traits::rep;
    using period     = std::nano;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<steady_clock>;

    static constexpr bool is_steady = true;

    static time_point now() noexcept {
        return time_point{duration{traits::monotonic_ticks()}};
    }
};

static_assert(Clock<steady_clock>,
              "steady_clock must satisfy Clock concept");

}