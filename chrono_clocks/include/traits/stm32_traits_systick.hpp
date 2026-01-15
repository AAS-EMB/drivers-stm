#pragma once

#include <cstdint>
#if defined(CHRONO_PLATFORM_STM32F1)
    #include "stm32f1xx.h"
#elif defined(CHRONO_PLATFORM_STM32F3)
    #include "stm32f3xx.h"
#else
    #error Unsupported platform
#endif

namespace driver {

struct chrono_traits {
    using rep    = int64_t;
    using period = std::nano;

    static void init(uint32_t cpu_freq_hz) noexcept {
        s_cpu_freq_hz = cpu_freq_hz;

        SysTick->LOAD = (cpu_freq_hz / 1000) - 1; // 1 ms tick
        SysTick->VAL  = 0;
        SysTick->CTRL =
            SysTick_CTRL_CLKSOURCE_Msk |
            SysTick_CTRL_ENABLE_Msk |
            SysTick_CTRL_TICKINT_Msk;

        s_ms_high = 0;
        s_last_val = SysTick->VAL;
    }

    static rep now() noexcept {
        uint32_t load = SysTick->LOAD + 1;
        uint32_t val  = SysTick->VAL;

        uint64_t ms = s_ms_high;

        // SysTick counts down
        if (val > s_last_val) {
            ++ms; // overflow (reload)
        }

        s_last_val = val;
        s_ms_high  = ms;

        uint32_t elapsed_cycles = load - val;
        uint64_t ns =
            (ms * 1'000'000ull) +
            (elapsed_cycles * 1'000'000ull) / load;

        return static_cast<rep>(ns);
    }

    // Call from SysTick_Handler
    static void on_tick() noexcept {
        ++s_ms_high;
    }

private:
    static inline uint32_t s_cpu_freq_hz = 0;
    static inline uint64_t s_ms_high     = 0;
    static inline uint32_t s_last_val    = 0;
};

}
