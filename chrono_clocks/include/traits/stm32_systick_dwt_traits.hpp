#pragma once

#include <cstdint>
#if defined(CHRONO_PLATFORM_STM32F7)
    #include "stm32f7xx.h"
#else
    #error Unsupported platform
#endif

namespace driver {

struct chrono_traits {
    using rep = uint64_t;

    static void init() noexcept {
        init_systick();
        init_dwt();
    }

    // monotonic time in ns
    static rep monotonic_ticks() noexcept {
        const uint32_t now = DWT->CYCCNT;

        if (now < dwt_last) {
            ++dwt_high;
        }

        dwt_last = now;

        const uint64_t cycles = (static_cast<uint64_t>(dwt_high) << 32) | now;

        return cycles_to_ns(cycles);
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

    // ---------- DWT (monotonic) ----------
    static inline uint32_t dwt_last = 0u;
    static inline uint32_t dwt_high = 0u;

    static void init_systick() noexcept {
        SysTick->LOAD = SystemCoreClock / 1000u - 1u;
        SysTick->VAL  = 0u;
        SysTick->CTRL =
            SysTick_CTRL_CLKSOURCE_Msk |
            SysTick_CTRL_TICKINT_Msk |
            SysTick_CTRL_ENABLE_Msk;
    }

    static void init_dwt() noexcept {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0u;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }

    static rep cycles_to_ns(uint64_t cycles) noexcept {
        return (cycles * 1'000'000'000ull) / SystemCoreClock;
    }
};

}