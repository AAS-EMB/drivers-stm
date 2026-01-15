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
    using rep = uint64_t;

    static void init() noexcept {
        init_systick();
    }

    // monotonic time in ns
    static rep monotonic_ticks() noexcept {
        uint32_t h1 = 0u, l = 0u, h2 = 0u, val = 0u;

        do {
            h1  = wall_high;
            l   = wall_low;
            val = SysTick->VAL;
            h2  = wall_high;
        } while (h1 not_eq h2);

        const uint32_t reload = SystemCoreClock / 1000u;

        const uint32_t sub = reload - val;

        // total ticks since boot
        const uint64_t total_ticks = (static_cast<uint64_t>(h1) << 32 | l) * reload + sub;

        // ticks → ns
        return static_cast<rep>(
            (total_ticks * 1'000'000'000ull) / SystemCoreClock
        );
    }

    // wall time in ms
    static rep wall_ticks() noexcept {
        uint32_t h1 = 0u, l = 0u, h2 = 0u;

        do {
            h1 = wall_high;
            l  = wall_low;
            h2 = wall_high;
        } while (h1 not_eq h2);

        return (static_cast<rep>(h1) << 32) | l;
    }

    static void systick_irq() noexcept {
        if (++wall_low == 0u) {
            ++wall_high;
        }
    }

private:
    // ---------- SysTick (wall time) ----------
    static inline volatile uint32_t wall_low  = 0u;
    static inline volatile uint32_t wall_high = 0u;

    static void init_systick() noexcept {
        SysTick->LOAD = SystemCoreClock / 1000u - 1u;
        SysTick->VAL  = 0u;
        SysTick->CTRL =
            SysTick_CTRL_CLKSOURCE_Msk |
            SysTick_CTRL_TICKINT_Msk |
            SysTick_CTRL_ENABLE_Msk;
    }
};

}
