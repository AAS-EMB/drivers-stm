#include <platform/stm32/gpio.hpp>

void compile_check() {
    [[maybe_unused]] driver::gpio dummy_{ GPIOA, 0x0001 };
}