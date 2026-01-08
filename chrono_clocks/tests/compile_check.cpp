#include "steady_clock.hpp"

void compile_check() {
    auto t = driver::steady_clock::now();
    (void)t;
}
