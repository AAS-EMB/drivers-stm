#pragma once

#if defined(CHRONO_PLATFORM_STM32F7)
    #include "traits/stm32_systick_dwt_traits.hpp"
#elif defined(CHRONO_PLATFORM_STM32F3) or defined(CHRONO_PLATFORM_STM32F1)
    #include "traits/stm32_systick_traits.hpp"
#elif defined(CHRONO_PLATFORM_HOST)
    #if defined(BUILD_TESTS)
        #include "traits/fake_test_traits.hpp"
    #else
        #include "traits/host_traits.hpp"
    #endif
#else
    #error "Unknown chrono platform"
#endif