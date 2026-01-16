#pragma once

#include "gpio_base.hpp"
#if defined(PLATFORM_STM32F1)
    #include "stm32f1xx.h"
#elif defined(PLATFORM_STM32F3)
    #include "stm32f3xx.h"
#else
    #error Unsupported platform
#endif

namespace driver {

class gpio : gpio_base<gpio> {
public:
    void set() noexcept {
        
    }

    void reset() noexcept {
        
    }

    [[nodiscard]] bool is_set() const noexcept {
        return true;
    }


};

}