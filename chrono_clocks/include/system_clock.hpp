#pragma once

#include <chrono>
#include <cstdint>
#include "platform.hpp"
#include "clock_concept.hpp"

namespace driver {

class system_clock {
public:
    using traits     = chrono_traits;
    using rep        = typename traits::rep;
    using period     = std::milli;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<system_clock>;

    static constexpr bool is_steady = false;

    static time_point now() noexcept {
        return time_point{duration{traits::wall_ticks()}}; // utc time
    }
};

static_assert(Clock<system_clock>,
              "system_clock must satisfy Clock concept");

}