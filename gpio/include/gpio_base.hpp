#pragma once

#include <cstdbool>

namespace driver {

template<typename Derived>
class gpio_base {
public:
    void set() noexcept {
        static_cast<Derived&>(*this).set();
    }

    void reset() noexcept {
        static_cast<Derived&>(*this).reset();
    }

    [[nodiscard]] bool is_set() const noexcept {
        return static_cast<Derived const&>(*this).is_set();
    }
};

}