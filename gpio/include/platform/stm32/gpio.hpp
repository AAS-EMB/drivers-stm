#pragma once

#include "gpio_base.hpp"
#if defined(PLATFORM_STM32F1)
    #include "stm32f1xx.h"
#elif defined(PLATFORM_STM32F3)
    #include "stm32f3xx.h"
#elif defined(PLATFORM_STM32F7)
    #include "stm32f7xx.h"
#else
    #error Unsupported platform
#endif

namespace driver {

class gpio final : public IGpio {
    GPIO_TypeDef* port_;
    uint16_t pin_;

public:
    gpio(GPIO_TypeDef* port, uint16_t pin) noexcept
        : port_{ port }
        , pin_ { pin  }
    { }
    ~gpio() noexcept override = default;

    void set() noexcept override {
        port_->BSRR = pin_;
    }

    void reset() noexcept override {
        port_->BSRR = static_cast<uint32_t>(pin_) << 16;
    }

    bool is_set() const noexcept override {
        return (port_->IDR & pin_) not_eq 0;
    }

    void toggle() noexcept override {
        port_->ODR ^= pin_;
    }

};

}