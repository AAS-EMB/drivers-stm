message(STATUS "Configuring for STM32F7")

add_library(cmsis_core INTERFACE)

target_include_directories(cmsis_core
    INTERFACE
        ${CMAKE_SOURCE_DIR}/extern/CMSIS/CMSIS/Core/Include
)

target_compile_definitions(cmsis_core
    INTERFACE
        ARM_MATH_CM7
)

target_link_libraries(chrono_clocks
    INTERFACE
        cmsis_core
)

target_compile_definitions(chrono_clocks
    INTERFACE
        STM32F7
        CHRONO_PLATFORM_STM32F7
)

#target_compile_options(chrono_clocks
#    INTERFACE
#        -mcpu=cortex-m7
#        -ffunction-sections
#        -fdata-sections
#)

target_link_options(chrono_clocks
    INTERFACE
        -Wl,--gc-sections
)
