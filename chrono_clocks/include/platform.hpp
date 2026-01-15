#pragma once

#if defined(CHRONO_PLATFORM_STM32F7)
    #include "traits/stm32_traits_dwt.hpp"
#elif defined(CHRONO_PLATFORM_STM32F3) or defined(CHRONO_PLATFORM_STM32F1)
    #include "traits/stm32_traits_systick.hpp"
#elif defined(CHRONO_PLATFORM_HOST)
    #include "traits/host_traits.hpp"
#else
    #error "Unknown chrono platform"
#endif