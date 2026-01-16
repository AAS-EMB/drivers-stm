#include "steady_clock.hpp"
#include "system_clock.hpp"

void compile_check() {
    [[maybe_unused]] auto t1 = driver::steady_clock::now();
    [[maybe_unused]] auto t2 = driver::system_clock::now();
}
