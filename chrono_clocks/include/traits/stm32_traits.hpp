#pragma once

#include <cstdint>
#include "core_cm7.h"

namespace stm32_drivers {

struct chrono_traits {
    using rep    = int64_t;
    using period = std::nano;

    static void init(uint32_t cpu_freq_hz) noexcept {
        s_cpu_freq_hz = cpu_freq_hz;
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        s_last = 0;
        s_high = 0;
    }

    static rep now() noexcept {
        __disable_irq();

        const uint32_t now  = DWT->CYCCNT;
        const uint32_t prev = s_last;

        if (now < prev) {
            ++s_high;   // overflow detected
        }

        s_last = now;

        const uint64_t cycles = (static_cast<uint64_t>(s_high) << 32) | now;

        __enable_irq();

        return cycles_to_ns(cycles);
    }

private:
    static inline uint32_t s_cpu_freq_hz = 168'000'000;
    static inline uint32_t s_last = 0;
    static inline uint32_t s_high = 0;

    static rep cycles_to_ns(uint64_t cycles) noexcept {
        return static_cast<rep>(
            (cycles * 1'000'000'000ull) / s_cpu_freq_hz
        );
    }
};

}
