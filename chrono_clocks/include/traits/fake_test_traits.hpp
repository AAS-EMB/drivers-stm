#pragma once

#include <chrono>

namespace driver {

struct chrono_traits {
    using rep = int64_t;

    static inline rep ticks = 0;

    static rep monotonic_ticks() noexcept {
        return ticks;
    }

    static rep wall_ticks() noexcept {
        return ticks;
    }

    static void advance(rep delta) noexcept {
        ticks += delta;
    }

    static void reset() noexcept {
        ticks = 0;
    }
    
};

}