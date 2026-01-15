#pragma once

#include <chrono>

namespace driver {

struct chrono_traits {
    using rep = uint64_t;

    static void init() noexcept {}

    static rep monotonic_ticks() noexcept {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(
            steady_clock::now().time_since_epoch()
        ).count();
    }

    static rep wall_ticks() noexcept {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }
};

}