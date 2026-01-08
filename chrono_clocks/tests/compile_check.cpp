#include "steady_clock.hpp"

void compile_check() {
    auto t = stm32_drivers::steady_clock::now();
    (void)t;
}
