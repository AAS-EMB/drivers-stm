#pragma once

#include <chrono>

namespace driver {

struct chrono_traits {
    using rep    = std::chrono::steady_clock::rep;
    using period = std::chrono::steady_clock::period;

    static rep now() noexcept {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

}