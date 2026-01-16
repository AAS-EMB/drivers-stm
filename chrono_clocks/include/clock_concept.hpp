#pragma once

#include <chrono>
#include <concepts>

namespace driver {

template<typename C>
concept Clock =
    requires {
        typename C::rep;
        typename C::period;
        typename C::duration;
        typename C::time_point;
    }
    &&
    requires {
        { C::is_steady } -> std::convertible_to<bool>;
    }
    &&
    requires {
        { C::now() } -> std::same_as<typename C::time_point>;
    };

}
